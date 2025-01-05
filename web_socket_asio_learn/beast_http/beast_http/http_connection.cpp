#include "http_connection.h"
namespace my_program_state {
	std::size_t request_count()
	{
		static std::size_t count = 0;
		return ++count;
	}

	std::time_t now()
	{
		return std::time(0);
	}
}

http_connection::http_connection(tcp::socket socket, boost::asio::io_context& ioc)
	: socket_(std::move(socket)), ws_ptr_(nullptr), _ioc(ioc), _strand(ioc.get_executor())
{
}

void http_connection::handle_error(boost::system::error_code ec)
{
	std::cerr << "WebSocket error: " << ec.message() << "\n";
	// Do something to handle the error, such as logging or shutting down the connection
}

void http_connection::start()
{
	read_request();
	check_deadline();
}

void http_connection::convert_websocket(tcp::socket&& socket) {
	ws_ptr_.reset(new stream<tcp_stream>(std::move(socket)));
}

void http_connection::read_request()
{
	auto self = shared_from_this();

	http::async_read(
		socket_,
		buffer_,
		request_,
		[self](beast::error_code ec,
			std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);
			if (ec) {
				std::cout << "async_read error ! msg is : " << ec.message() << std::endl;
				return;
			}
			if (websocket::is_upgrade(self->request_)) {
				self->upgrade_websocket(std::move(self->socket_), bytes_transferred);
			}
			else {
				self->process_request();
			}
		});
}


void http_connection::process_request()
{
	response_.version(request_.version());
	response_.keep_alive(false);

	switch (request_.method())
	{
	case http::verb::get:
		response_.result(http::status::ok);
		response_.set(http::field::server, "Beast");
		create_response();
		break;

	default:
		// We return responses indicating an error if
		// we do not recognize the request method.
		response_.result(http::status::bad_request);
		response_.set(http::field::content_type, "text/plain");
		beast::ostream(response_.body())
			<< "Invalid request-method '"
			<< std::string(request_.method_string())
			<< "'";
		break;
	}

	write_response();
}


// Construct a response message based on the program state.
void http_connection::create_response()
{
	if (request_.target() == "/count")
	{
		response_.set(http::field::content_type, "text/html");
		beast::ostream(response_.body())
			<< "<html>\n"
			<< "<head><title>Request count</title></head>\n"
			<< "<body>\n"
			<< "<h1>Request count</h1>\n"
			<< "<p>There have been "
			<< my_program_state::request_count()
			<< " requests so far.</p>\n"
			<< "</body>\n"
			<< "</html>\n";
	}
	else if (request_.target() == "/time")
	{
		response_.set(http::field::content_type, "text/html");
		beast::ostream(response_.body())
			<< "<html>\n"
			<< "<head><title>Current time</title></head>\n"
			<< "<body>\n"
			<< "<h1>Current time</h1>\n"
			<< "<p>The current time is "
			<< my_program_state::now()
			<< " seconds since the epoch.</p>\n"
			<< "</body>\n"
			<< "</html>\n";
	}
	else
	{
		response_.result(http::status::not_found);
		response_.set(http::field::content_type, "text/plain");
		beast::ostream(response_.body()) << "File not found\r\n";
	}
}

// Asynchronously transmit the response message.
void http_connection::write_response()
{
	auto self = shared_from_this();

	response_.content_length(response_.body().size());

	http::async_write(
		socket_,
		response_,
		[self](beast::error_code ec, std::size_t)
		{
			self->socket_.shutdown(tcp::socket::shutdown_send, ec);
			self->deadline_.cancel();
		});
}

// Check whether we have spent enough time on this connection.
void http_connection::check_deadline()
{
	auto self = shared_from_this();

	deadline_.async_wait(
		[self](beast::error_code ec)
		{
			if (!ec)
			{
				// Close socket to cancel any outstanding operation.
				self->socket_.close(ec);
			}
		});
}

void http_connection::stop_deadline() {
	deadline_.cancel();
}

void http_connection::upgrade_websocket(const tcp::socket&& socket, std::size_t bytes_transferred) {
	auto self = shared_from_this();
	// Construct the stream, transferring ownership of the socket
	stop_deadline();
	convert_websocket(std::move(socket_));
	// Clients SHOULD NOT begin sending WebSocket
	// frames until the server has provided a response.
	BOOST_ASSERT(_read_buffer.size() == 0);

	// Accept the upgrade request
	// Accept the upgrade request asynchronously
	self->ws_ptr_->async_accept(self->request_,
		[self](boost::system::error_code ec)
		{
			if (!ec)
			{
				self->websocket_async_read();
			}
			else
			{
				// An error occurred while accepting the WebSocket connection.
				self->handle_error(ec);
				get_lowest_layer(*(self->ws_ptr_)).close();
			}
		});
}

void http_connection::async_send_message(const std::string& message)
{
	size_t que_size = 0;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		que_size = _send_que.size();
		_send_que.push(message);
	}

	if (que_size > 0) {
		return;
	}

	async_write(message);
}

void http_connection::async_write(const std::string& message) {
	auto self = shared_from_this();
	self->ws_ptr_->async_write(boost::asio::buffer(message.c_str(), message.length()), boost::asio::bind_executor(_strand,
		[self](boost::system::error_code ec, std::size_t bytes_transferred)
		{
			try {
				if (!ec)
				{
					std::string send_msg;
					// Message sent successfully
					{
						std::lock_guard<std::mutex> lock(self->_mutex);
						self->_send_que.pop();
						if (self->_send_que.empty()) {
							return;
						}

						send_msg = self->_send_que.front();
					}
					self->async_write(send_msg);
				}
				else
				{
					// An error occurred while sending the message
					self->handle_error(ec);
					get_lowest_layer(*(self->ws_ptr_)).close();
				}
			}
			catch (std::exception const& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				return;
			}
		}));
}

void http_connection::websocket_async_read() {
	auto self = shared_from_this();
	// Read a complete message into the buffer's input area asynchronously
	self->ws_ptr_->async_read(self->_read_buffer, boost::asio::bind_executor(_strand, [self](boost::system::error_code ec, std::size_t bytes_transferred)
		{
			try {
				if (!ec)
				{
					// Set text mode if the received message was also text,
					// otherwise binary mode will be set.
					self->ws_ptr_->text(self->ws_ptr_->got_text());

					// Echo the received message back to the peer. If the received
					// message was in text mode, the echoed message will also be
					// in text mode, otherwise it will be in binary mode.
					const auto& recv_data = boost::beast::buffers_to_string(self->_read_buffer.data());
					std::cout << "websocket receive data is " << recv_data << std::endl;
					self->async_send_message(recv_data);

					// Discard all of the bytes stored in the dynamic buffer.
					self->_read_buffer.consume(self->_read_buffer.size());
					self->websocket_async_read();
				}
				else
				{
					self->handle_error(ec);
					// An error occurred while reading from the WebSocket connection.
					//self->ws_ptr_->close(websocket::close_code::none);
					get_lowest_layer(*(self->ws_ptr_)).close();
				}
			}
			catch (std::exception const& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
				return;
			}
		})
	);
}
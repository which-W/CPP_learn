#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <boost/beast/websocket.hpp>
#include <queue>
#include <mutex>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using namespace boost::beast;
using namespace boost::beast::websocket;
namespace my_program_state
{
	extern std::size_t
		request_count();

	extern std::time_t now();
}

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
	http_connection(tcp::socket socket, boost::asio::io_context& ioc);

	// Handle WebSocket error
	void handle_error(boost::system::error_code ec);

	// Initiate the asynchronous operations associated with the connection.
	void  start();

	void convert_websocket(tcp::socket&& socket);
	void async_send_message(const std::string& message);
private:
	// The socket for the currently connected client.
	tcp::socket socket_;
	std::unique_ptr<stream<tcp_stream>> ws_ptr_;
	// The buffer for performing reads.
	beast::flat_buffer buffer_{ 8192 };

	// The request message.
	http::request<http::string_body> request_;

	// The response message.
	http::response<http::dynamic_body> response_;

	// The timer for putting a deadline on connection processing.
	net::steady_timer deadline_{
		socket_.get_executor(), std::chrono::seconds(60) };

	// Asynchronously receive a complete request message.
	void read_request();

	// Determine what needs to be done with the request message.
	void process_request();

	// Construct a response message based on the program state.
	void create_response();

	// Asynchronously transmit the response message.
	void write_response();

	// Check whether we have spent enough time on this connection.
	void check_deadline();

	void stop_deadline();

	void upgrade_websocket(const tcp::socket&& socket, std::size_t bytes_transferred);
	void websocket_async_read();

	void async_write(const std::string& message);
	flat_buffer _read_buffer;
	std::queue<std::string> _send_que;
	std::mutex  _mutex;
	boost::asio::strand<boost::asio::io_context::executor_type> _strand;
	boost::asio::io_context& _ioc;
};

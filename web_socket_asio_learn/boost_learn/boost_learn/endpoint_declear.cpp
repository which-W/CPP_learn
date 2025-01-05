#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>
using namespace boost;

int client_end_point(std::string raw_ip_addr = "127.0.0.1" ,unsigned short port_num = 3333) {
	boost::system::error_code ec;
	asio::ip::address ip_addr = asio::ip::address::from_string(raw_ip_addr,ec);
	if (ec.value() != 0) {
		std::cout << "failed to parse the ip address. error code = " << ec.value() << ".message"<<ec.message()<<std::endl;
		return ec.value();
	}
	asio::ip::tcp::endpoint pc(ip_addr, port_num);
	return 0;
}

int server_end_point(unsigned short port_num = 3333) {
	asio::ip::address ip_addr = asio::ip::address_v4::any();
	asio::ip::tcp::endpoint sc(ip_addr, port_num);
	return 0;
}

int create_socket_tcp() {
	asio::io_context iox;
	asio::ip::tcp protocol = asio::ip::tcp::v4();
	boost::system::error_code ec;
	asio::ip::tcp::socket server_socket(iox);
	server_socket.open(protocol, ec);
	if (ec.value() != 0) {
		std::cout << "failed to open socket . error code = " 
		<< ec.value() << ".message" << ec.message() << std::endl;
		return ec.value();
	}
	return 0;
	//asio::ip::tcp::acceptor ac(iox, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), 3333));
}

int  bind_acceptor_socket() {
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),
		port_num);
	asio::io_context  ios;
	asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
	boost::system::error_code ec;
	acceptor.bind(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to bind the acceptor socket."
			<< "Error code = " << ec.value() << ". Message: "
			<< ec.message();
		return ec.value();
	}
	return 0;
}

int connect_to_end() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
				port_num);
		asio::io_context ios;
		asio::ip::tcp::socket sock(ios, ep.protocol()); 
		sock.connect(ep);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();
		return e.code().value();
	}
}

int accept_new_connection() {
	const int BACKLOG_SIZE = 30;
	unsigned short port_num = 3333;

	asio::ip::tcp::endpoint ep (asio::ip::address_v4::any(), port_num);
	asio::io_context iox;

	try
	{
		asio::ip::tcp::acceptor ac(iox, ep.protocol());
		ac.bind(ep);

		ac.listen(BACKLOG_SIZE);

		asio::ip::tcp::socket socket(iox); //通信的socket
		ac.accept(socket);
	}
	catch (system::system_error & ec)
	{
		std::cout << "Error occured! Error code = " << ec.code()
			<< ". Message: " << ec.what();
		return ec.code().value();
	}
}

void use_const_buf() {
	std::string buf = "hello world";
	asio::const_buffer asio_buf(buf.c_str(), buf.length());
	std::vector<asio::const_buffer> const_buffer_queue;
	const_buffer_queue.push_back(asio_buf);
}

void use_buffer_str() {
	asio::const_buffers_1 output_buf = asio::buffer("hello world");
}

void use_buffer_array() {
	const size_t BUF_SIZE_BYTE = 20;
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTE]);
	auto input = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTE);

}


void write_to_socket(asio::ip::tcp::socket & socket) {
	std::string buf = "hello world";
	std::size_t total_byte_written = 0;
	while (total_byte_written != buf.length()) {
		total_byte_written += socket.write_some(asio::buffer(buf.c_str() + total_byte_written,
			buf.length() - total_byte_written));
	}//比较重要的写代码
}


int send_data_by_write_some() {
	std::string raw_ip_addr = "127.0.0.1";
	unsigned short ports_num = 3333;
	try
	{
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_addr), ports_num);
		asio::io_context iox;
		asio::ip::tcp::socket socket(iox, ep.protocol());
		socket.connect(ep);
		write_to_socket(socket);
	}
	catch (system::system_error & ec)
	{
		std::cout << "Error occured! Error code = " << ec.code()
			<< ". Message: " << ec.what();
		return ec.code().value();
	}
}


int send_data_by_send() {
	std::string raw_ip_addr = "127.0.0.1";
	unsigned short ports_num = 3333;
	try
	{
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_addr), ports_num);
		asio::io_context iox;
		asio::ip::tcp::socket socket(iox, ep.protocol());
		socket.connect(ep);
		std::string buf = "hello world";
		int socket_length = socket.send(asio::buffer(buf.c_str(), buf.length()));
		if (socket_length <= 0) {
			return 0;
		}
	}
	catch (system::system_error& ec)
	{
		std::cout << "Error occured! Error code = " << ec.code()
			<< ". Message: " << ec.what();
		return ec.code().value();
	}
}

int send_data_by_write() {
	std::string raw_ip_addr = "127.0.0.1";
	unsigned short ports_num = 3333;
	try
	{
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_addr), ports_num);
		asio::io_context iox;
		asio::ip::tcp::socket socket(iox, ep.protocol());
		socket.connect(ep);
		std::string buf = "hello world";
		int socket_length = asio::write(socket,asio::buffer(buf.c_str(), buf.length()));
		if (socket_length <= 0) {
			return 0;
		}
	}
	catch (system::system_error& ec)
	{
		std::cout << "Error occured! Error code = " << ec.code()
			<< ". Message: " << ec.what();
		return ec.code().value();
	}
}

std::string read_from_socket(asio::ip::tcp::socket& sock) {
	const unsigned char MESSAGE_SIZE = 7;
	char buf[MESSAGE_SIZE];
	std::size_t total_bytes_read = 0;
	while (total_bytes_read != MESSAGE_SIZE) {
		total_bytes_read += sock.read_some(
			asio::buffer(buf + total_bytes_read,
				MESSAGE_SIZE - total_bytes_read));
	}
	return std::string(buf, total_bytes_read);
}

int read_data_by_read_some() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
				port_num);
		asio::io_service ios;
		asio::ip::tcp::socket sock(ios, ep.protocol());
		sock.connect(ep);
		read_from_socket(sock);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();
		return e.code().value();
	}
	return 0;
}

int read_data_by_recive() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
				port_num);
		asio::io_service ios;
		asio::ip::tcp::socket sock(ios, ep.protocol());
		sock.connect(ep);
		const unsigned char MESSAGE_LEG = 5;
		char buffer_recive[MESSAGE_LEG];
		int receive_length = sock.receive(asio::buffer(buffer_recive, MESSAGE_LEG));
		if (receive_length <= 0) {
			return 0;
		}
	}
	catch (system::system_error& ec) {
		std::cout << "Error occured! Error code = " << ec.code()
			<< ". Message: " << ec.what();
		return ec.code().value();
	}
	return 0;
}

int read_data_by_recive() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
				port_num);
		asio::io_service ios;
		asio::ip::tcp::socket sock(ios, ep.protocol());
		sock.connect(ep);
		const unsigned char MESSAGE_LEG = 5;
		char buffer_recive[MESSAGE_LEG];
		int receive_length = asio::read(sock,asio::buffer(buffer_recive, MESSAGE_LEG));
		if (receive_length <= 0) {
			return 0;
		}
	}
	catch (system::system_error& ec) {
		std::cout << "Error occured! Error code = " << ec.code()
			<< ". Message: " << ec.what();
		return ec.code().value();
	}
	return 0;
}
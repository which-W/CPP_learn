//

#include <boost/beast/core.hpp>﻿
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>


// "Loop" forever accepting new connections.
#include "http_connection.h"


void http_server(tcp::acceptor& acceptor, tcp::socket& socket)
{
	acceptor.async_accept(socket,
		[&](beast::error_code ec)
		{
			if (!ec)
				std::make_shared<http_connection>(std::move(socket))->Start();
			http_server(acceptor, socket);
		});
}

int
main(int argc, char* argv[])
{
	try
	{
		// Check command line arguments.
		/*if (argc != 3)
		{
			std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    receiver 0.0.0.0 80\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    receiver 0::0 80\n";
			return EXIT_FAILURE;
		}*/

		auto const address = net::ip::make_address("0.0.0.0");
		unsigned short port = static_cast<unsigned short>(8080);

		net::io_context ioc{ 1 };

		tcp::acceptor acceptor{ ioc, {address, port} };
		tcp::socket socket{ ioc };
		http_server(acceptor, socket);

		ioc.run();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
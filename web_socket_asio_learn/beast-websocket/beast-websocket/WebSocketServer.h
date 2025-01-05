#pragma once
#include "Connection.h"
class WebSocketServer
{
public:
	WebSocketServer(const WebSocketServer&) = delete;
	WebSocketServer& operator = (const WebSocketServer&) = delete;
	WebSocketServer(net::io_context& ioc, short port);
	void StartAccept();
private:
	net::io_context& _ioc;
	net::ip::tcp::acceptor _acceptor;
};


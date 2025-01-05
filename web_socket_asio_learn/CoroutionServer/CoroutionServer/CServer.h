#pragma once
#include"CSession.h"
#include<boost/asio.hpp>
#include<map>
#include<mutex>
class CServer
{
public:
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	void ClearSession(std::string);
private:
	void HandleAccept(std::shared_ptr<CSession>CSession ,const boost::system::error_code& error );
	void StartAccept();
	short _port;
	boost::asio::io_context& _io_context;
	boost::asio::ip::tcp::acceptor _acceptor;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mutex;
};


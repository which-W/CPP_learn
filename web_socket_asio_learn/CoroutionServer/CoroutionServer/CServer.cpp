#include "CServer.h"
#include "AsioIOServicePool.h"
#include <iostream>
CServer::CServer(boost::asio::io_context& io_context, short port) :_io_context(io_context),
_port(port), _acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	StartAccept();
}

CServer::~CServer() {

}

void CServer::StartAccept() {
	auto& io_context = AsioIOServicePool::GetInstance().GetIOService();
	std::shared_ptr<CSession> New_Session = std::make_shared<CSession>(io_context, this);
	_acceptor.async_accept(New_Session->GetSocket(),
		std::bind(&CServer::HandleAccept, this, New_Session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession>CSession, const boost::system::error_code& error) {
	if (!error) {
		CSession->Start();
		std::lock_guard<std::mutex>lock(_mutex);
		_sessions.insert(std::make_pair(CSession->GetUuid(), CSession));
	}
	else
	{
		std::cout << "session accept is error" << error.what() << std::endl;
	}

	StartAccept();
}

void CServer::ClearSession(std::string uuid) {
	std::lock_guard<std::mutex>lock(_mutex);
	_sessions.erase(uuid);
}
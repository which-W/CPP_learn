#pragma once
#include <iostream>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <memory>
#include <mutex>
#include <queue>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace net = boost::asio;
namespace beast = boost::beast;
using namespace boost::beast::websocket;
using namespace boost::beast;

class ConnectionMgr;
class Connection: public std::enable_shared_from_this<Connection>
{
public:
	Connection(boost::asio::io_context& ioc);
	std::string GetID();
	net::ip::tcp::socket& GetSocket();
	void AsynAccept();
	void Start();
	void AsynSend(std::string msg);
	void SendCallBack(std::string msg);
private:
	std::unique_ptr<stream<tcp_stream>> _ws_ptr;
	boost::asio::io_context& _ioc;
	std::string _uuid;
	std::mutex _mutex;
	std::queue<std::string> _send_que;
	flat_buffer _recv_buffer;

};


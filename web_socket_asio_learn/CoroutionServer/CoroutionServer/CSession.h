#pragma once
#include<boost/asio.hpp>
#include<boost/uuid/uuid_generators.hpp>
#include<boost/uuid/uuid_io.hpp>
#include<boost/asio/co_spawn.hpp>
#include<boost/asio/detached.hpp>
#include "const.h"
#include <queue>
#include <mutex>
#include <memory>
#include "MsgNode.h"


using boost::asio::ip::tcp;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::strand;
using boost::asio::io_context;
namespace this_coro = boost::asio::this_coro;

class CServer;
class CSession : std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	~CSession();
	boost::asio::ip::tcp::socket& GetSocket();
	void Start();
	std::string& GetUuid();
	void Close();
	void Send(const char* msg, short msg_id, short msg_len);
	void Send(std::string msg, short msg_id);
private:
	void HandleWrite(const boost::system::error_code & error , std::shared_ptr<CSession> shared_self);
	boost::asio::io_context& _io_context;
	CServer* _server;
	boost::asio::ip::tcp::socket _socket;
	std::string _uuid;
	bool _b_close;
	std::mutex _send_lock;
	std::queue<std::shared_ptr<SendNode>> _send_queue;
	std::shared_ptr<RecvNode> _recv_msg_node;
	std::shared_ptr<MsgNode> _recv_head_node;


};

class LogicNode {
public:
	LogicNode(std::shared_ptr<CSession> CSession, std::shared_ptr<RecvNode> SendNode);
	std::shared_ptr<CSession> _session;
	std::shared_ptr<RecvNode> _recvnode;
};
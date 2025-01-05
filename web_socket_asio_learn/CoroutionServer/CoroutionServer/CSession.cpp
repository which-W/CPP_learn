#include "CSession.h"
#include "CServer.h"
#include "LogicSystem.h"
CSession::CSession(boost::asio::io_context& io_context, CServer* server) :_io_context(io_context),
_server(server),_socket(io_context),_b_close(false) {
	boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); //这里需要注意一下
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

boost::asio::ip::tcp::socket& CSession::GetSocket() {
	return _socket;
}

std::string& CSession::GetUuid() {
	return _uuid;
}

void CSession::Start() {
	auto shared_this = shared_from_this();
	boost::asio::co_spawn(_io_context, [shared_this, this]()->boost::asio::awaitable<void> {
		try {
			for (; !_b_close;) {
				_recv_head_node->Clear();
				size_t n = co_await boost::asio::async_read(_socket,boost::asio::buffer(_recv_head_node->_data, HEAD_TOTAL_LEN), 
					use_awaitable);

				if (n == 0) {
					std::cout << "conection is close" << std::endl;
					Close();
					_server->ClearSession(_uuid);
					co_return;
				}

				//获取头部MSG信息
				short msg_id = 0;
				memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
				msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
				if (msg_id > MAX_LENGTH) {
					std::cout << " invaild msg_id " << std::endl;
					Close();
					_server->ClearSession(_uuid);
					co_return;
				}

				short msg_len = 0;
				memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN,
					HEAD_DATA_LEN);
				msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
				if (msg_len > MAX_LENGTH) {
					std::cout << " invaild msg_len" << std::endl;
					Close();
					_server->ClearSession(_uuid);
					co_return;
				}

				_recv_msg_node = std::make_shared<RecvNode>(msg_len, msg_id);

				//读出包体
				n = co_await boost::asio::async_read(_socket, boost::asio::buffer(_recv_msg_node->_data, _recv_msg_node->_total_len),
					use_awaitable);
				if (n == 0) {
					std::cout << "msg is empty ,connect is false" << std::endl;
					Close();
					_server->ClearSession(_uuid);
					co_return;
				}
				_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
				std::cout << "receive data is " << _recv_msg_node->_data << std::endl;
				//投递给逻辑线程
				LogicSystem::GetInstance().PostMsgToQue(std::make_shared<LogicNode>(shared_from_this(), _recv_msg_node));
			}
		}
		catch (std::exception &ec) {
			std::cout << "Exception is" << ec.what() << std::endl;
			Close();
			_server->ClearSession(_uuid);
		}
		},detached);
}

void CSession::Close() {
	_socket.close();
	_b_close = true;
}

CSession::~CSession() {
	try {
		Close();
	}
	catch (std::exception &ec) {
		std::cout << "Execption is " << ec.what() << std::endl;
	}
}

void CSession::Send(std::string msg, short msgid) {
	Send(msg.c_str(), msg.length(), msgid);
}

void CSession::Send(const char* msg, short msg_id, short msg_len) {
	std::unique_lock<std::mutex> lock(_send_lock);
	int send_que_len = _send_queue.size();
	if (send_que_len > MAX_SENDQUE) {
		std::cout << "send_queue is full" << std::endl;
		return;
	}
	_send_queue.push(std::make_shared<SendNode>(msg, msg_len, msg_id));
	if (send_que_len > 0) { //证明没有处理完队列数据则返回
		return;
	}
	auto msgnode = _send_queue.front();
	lock.unlock();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));

}

void CSession::HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self) {
	try {
		if (!error) {
			std::unique_lock<std::mutex> lock(_send_lock);
			_send_queue.pop();
			if (!_send_queue.empty()) {
				auto msgnode = _send_queue.front();
				lock.unlock();
				boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
					std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
			}
		}
		else {
			std::cout << "connect is error HandleWrite is error ,error is "<< error.what() << std::endl;
			Close();
			_server->ClearSession(_uuid);
		}
	}
	catch (std::exception &ec) {
		std::cout << "Exeption is " << ec.what() << std::endl;
		Close();
		_server->ClearSession(_uuid);
	}
}

LogicNode::LogicNode(std::shared_ptr<CSession> CSession, std::shared_ptr<RecvNode> RecvNode) :_session(CSession), _recvnode(RecvNode) {

}
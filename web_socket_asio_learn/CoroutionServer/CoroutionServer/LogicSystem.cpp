#include "LogicSystem.h"

LogicSystem::LogicSystem() :_b_stop(false) {
	RegisterCallBack();
	_worker_threads = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::RegisterCallBack() {
	_fun_callback[MSG_HELLO_WORD] = std::bind(&LogicSystem::HelloWordCallBack, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::DealMsg() {
	for (;;)
	{
		std::unique_lock<std::mutex> unique_lk(_mutex);
		while (_msg_que.empty() && !_b_stop) {
			_consume.wait(unique_lk);
		}
		if (_b_stop) {
			while (!_msg_que.empty()) {
				auto msg_node = _msg_que.front();
				auto _call_back_iter = _fun_callback.find(msg_node->_recvnode->GetRecvID());
				if (_call_back_iter == _fun_callback.end()) {
					_msg_que.pop();
					continue;
				}

				_call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetRecvID(),
					std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));

				_msg_que.pop();
			}
			break;
		}
		//没有停服并且队列有数据
		auto msg_node = _msg_que.front();

		auto _call_back_iter = _fun_callback.find(msg_node->_recvnode->GetRecvID());
		if (_call_back_iter == _fun_callback.end()) {
			_msg_que.pop();
			continue;
		}

		_call_back_iter->second(msg_node->_session, msg_node->_recvnode->GetRecvID(),
			std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_total_len));

		_msg_que.pop();
	}
	

}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg) {
	std::unique_lock<std::mutex> unique_lk(_mutex);
	_msg_que.push(msg);
	//由0变为1则发送通知信号
	if (_msg_que.size() == 1) {
		unique_lk.unlock();
		_consume.notify_one();
	}
}

void LogicSystem::HelloWordCallBack(std::shared_ptr<CSession> session,
	const short& msg_id, const std::string& msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
		<< root["data"].asString() << std::endl;
	root["data"] = "server has received msg, msg data is " + root["data"].asString();
	std::string return_str = root.toStyledString();
	session->Send(return_str, root["id"].asInt());
}

LogicSystem::~LogicSystem() {
	_b_stop = true;
	_consume.notify_one();
	_worker_threads.join();
}

LogicSystem& LogicSystem::GetInstance() {
	static LogicSystem instance;
	return instance;
}
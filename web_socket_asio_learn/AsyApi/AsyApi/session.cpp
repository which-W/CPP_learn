#include "session.h"

session::session(std::shared_ptr<asio::ip::tcp::socket> socket):_socket(socket),_send_pending(false), _recv_pending(false){

}

void session::connect(asio::ip::tcp::endpoint& ep) {
	_socket->connect(ep);
}

void session::WriteToSocketErr(const std::string& buf) {
    _send_node = std::make_shared<MsgNode>(buf.c_str(), buf.length());
    //异步发送数据，因为异步所以不会一下发送完
    this->_socket->async_write_some(asio::buffer(_send_node->_msg,_send_node->_total_len),
        std::bind(&session::WriteCallBackErr,this, std::placeholders::_1, std::placeholders::_2, _send_node));
}

void session::WriteCallBackErr(const boost::system::error_code& ec,
    std::size_t bytes_transferred, std::shared_ptr<MsgNode> msg_node) {
    if (bytes_transferred + msg_node->_cur_len< msg_node->_total_len) {
        _send_node->_cur_len += bytes_transferred;
        this->_socket->async_write_some(asio::buffer(_send_node->_msg + _send_node->_cur_len,
            _send_node->_total_len - _send_node->_cur_len),
            std::bind(&session::WriteCallBackErr,
                this, std::placeholders::_1, std::placeholders::_2, _send_node));
    }
}


void session::WriteToSocket(const std::string& buf) {
    //插入发送队列
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.length()));
    //pending状态说明上一次有未发送完的数据
    if (_send_pending) {
        return;
    }
    this->_socket->async_read_some(asio::buffer(buf), 
    std::bind(&session::WriteCallBack, this, std::placeholders::_1, std::placeholders::_2));

    _send_pending = true;
}

void session::WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error , code is " << ec.value();
        std::cout << " . Message is " << ec.message();
        return;
    }
    //取出队首元素即当前未发送完数据
    auto send_data = _send_queue.front();
    send_data->_cur_len += bytes_transferred;

    //数据未发送完， 则继续发送
    if (send_data->_cur_len < send_data->_total_len) {
        this->_socket->async_write_some(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&session::WriteCallBack,this, std::placeholders::_1, std::placeholders::_2));
        return;
    }

    //如果发送完，则pop出队首元素
    _send_queue.pop();
    //如果队列为空，则说明所有数据都发送完,将pending设置为false
    if (_send_queue.empty()) {
        _send_pending = false;
    }
    //如果队列不是空，则继续将队首元素发送
    if (!_send_queue.empty()) {
        auto& send_data = _send_queue.front();
        this->_socket->async_write_some(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&session::WriteCallBack,this,std::placeholders::_1, std::placeholders::_2));
    }

}

//不能与async_write_some混合使用
void session::WriteAllToSocket(const std::string& buf) {
    //插入发送队列
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.length()));
    //pending状态说明上一次有未发送完的数据
    if (_send_pending) {
        return;
    }
    //异步发送数据，因为异步所以不会一下发送完
    this->_socket->async_send(asio::buffer(buf),
        std::bind(&session::WriteAllCallBack, this,
            std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}
void session::WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error occured! Error code = "
            << ec.value()
            << ". Message: " << ec.message();
        return;
    }
    //如果发送完，则pop出队首元素
    _send_queue.pop();
    //如果队列为空，则说明所有数据都发送完,将pending设置为false
    if (_send_queue.empty()) {
        _send_pending = false;
    }
    //如果队列不是空，则继续将队首元素发送
    if (!_send_queue.empty()) {
        auto& send_data = _send_queue.front();
        this->_socket->async_send(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&session::WriteAllCallBack,
                this, std::placeholders::_1, std::placeholders::_2));
    }
}

void session::ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error , code is " << ec.value();
        std::cout << " . Message is " << ec.message();
        return;
    }
    auto read_data = _read_queue.front();
    read_data->_cur_len += bytes_transferred;
    //没读完继续读
    if (read_data->_cur_len < read_data->_total_len) {
        _socket->async_read_some(asio::buffer(read_data ->_msg + read_data->_cur_len,
            read_data->_total_len - read_data->_cur_len),
            std::bind(&session::ReadCallBack, this,std::placeholders::_1, std::placeholders::_2));
        return;
    }
    //如果读完，则pop出队首元素
    _read_queue.pop();
    //如果队列为空，则说明所有数据都读完,将pending设置为false
    if (_read_queue.empty()) {
        _recv_pending = false;
    }
    //如果队列不是空，则继续将队首元素发送
    if (!_read_queue.empty()) {
        auto& read_data = _read_queue.front();
        this->_socket->async_read_some(asio::buffer(read_data->_msg + read_data->_cur_len, read_data->_total_len - read_data->_cur_len),
            std::bind(&session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
    }
    //将数据投递到队列里交给逻辑线程处理，此处略去
    //如果读完了则将标记置为false
    //_recv_pending = false;
    
    //指针置空
    //_recv_node = nullptr;
}

void session::ReadFromSocket() {
    _read_queue.emplace(new MsgNode(RECVSIZE));
    if (_recv_pending) {
        return;
    }
    //可以调用构造函数直接构造，但不可用已经构造好的智能指针赋值
    /*auto _recv_nodez = std::make_unique<MsgNode>(RECVSIZE);
    _recv_node = _recv_nodez;*/
    //_recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_read_some(asio::buffer(_recv_node->_msg, _recv_node->_total_len), 
        std::bind(&session::ReadCallBack, this,std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}
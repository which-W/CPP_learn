#include "session.h"

session::session(std::shared_ptr<asio::ip::tcp::socket> socket):_socket(socket),_send_pending(false), _recv_pending(false){

}

void session::connect(asio::ip::tcp::endpoint& ep) {
	_socket->connect(ep);
}

void session::WriteToSocketErr(const std::string& buf) {
    _send_node = std::make_shared<MsgNode>(buf.c_str(), buf.length());
    //�첽�������ݣ���Ϊ�첽���Բ���һ�·�����
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
    //���뷢�Ͷ���
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.length()));
    //pending״̬˵����һ����δ�����������
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
    //ȡ������Ԫ�ؼ���ǰδ����������
    auto send_data = _send_queue.front();
    send_data->_cur_len += bytes_transferred;

    //����δ�����꣬ ���������
    if (send_data->_cur_len < send_data->_total_len) {
        this->_socket->async_write_some(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&session::WriteCallBack,this, std::placeholders::_1, std::placeholders::_2));
        return;
    }

    //��������꣬��pop������Ԫ��
    _send_queue.pop();
    //�������Ϊ�գ���˵���������ݶ�������,��pending����Ϊfalse
    if (_send_queue.empty()) {
        _send_pending = false;
    }
    //������в��ǿգ������������Ԫ�ط���
    if (!_send_queue.empty()) {
        auto& send_data = _send_queue.front();
        this->_socket->async_write_some(asio::buffer(send_data->_msg + send_data->_cur_len, send_data->_total_len - send_data->_cur_len),
            std::bind(&session::WriteCallBack,this,std::placeholders::_1, std::placeholders::_2));
    }

}

//������async_write_some���ʹ��
void session::WriteAllToSocket(const std::string& buf) {
    //���뷢�Ͷ���
    _send_queue.emplace(new MsgNode(buf.c_str(), buf.length()));
    //pending״̬˵����һ����δ�����������
    if (_send_pending) {
        return;
    }
    //�첽�������ݣ���Ϊ�첽���Բ���һ�·�����
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
    //��������꣬��pop������Ԫ��
    _send_queue.pop();
    //�������Ϊ�գ���˵���������ݶ�������,��pending����Ϊfalse
    if (_send_queue.empty()) {
        _send_pending = false;
    }
    //������в��ǿգ������������Ԫ�ط���
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
    //û���������
    if (read_data->_cur_len < read_data->_total_len) {
        _socket->async_read_some(asio::buffer(read_data ->_msg + read_data->_cur_len,
            read_data->_total_len - read_data->_cur_len),
            std::bind(&session::ReadCallBack, this,std::placeholders::_1, std::placeholders::_2));
        return;
    }
    //������꣬��pop������Ԫ��
    _read_queue.pop();
    //�������Ϊ�գ���˵���������ݶ�����,��pending����Ϊfalse
    if (_read_queue.empty()) {
        _recv_pending = false;
    }
    //������в��ǿգ������������Ԫ�ط���
    if (!_read_queue.empty()) {
        auto& read_data = _read_queue.front();
        this->_socket->async_read_some(asio::buffer(read_data->_msg + read_data->_cur_len, read_data->_total_len - read_data->_cur_len),
            std::bind(&session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
    }
    //������Ͷ�ݵ������ｻ���߼��̴߳����˴���ȥ
    //����������򽫱����Ϊfalse
    //_recv_pending = false;
    
    //ָ���ÿ�
    //_recv_node = nullptr;
}

void session::ReadFromSocket() {
    _read_queue.emplace(new MsgNode(RECVSIZE));
    if (_recv_pending) {
        return;
    }
    //���Ե��ù��캯��ֱ�ӹ��죬���������Ѿ�����õ�����ָ�븳ֵ
    /*auto _recv_nodez = std::make_unique<MsgNode>(RECVSIZE);
    _recv_node = _recv_nodez;*/
    //_recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_read_some(asio::buffer(_recv_node->_msg, _recv_node->_total_len), 
        std::bind(&session::ReadCallBack, this,std::placeholders::_1, std::placeholders::_2));
    _recv_pending = true;
}
#pragma once
#include<iostream>
#include<memory>
#include<boost/asio.hpp>
#include<queue>
using namespace boost;
//����Ľ��մ�С
const int RECVSIZE = 1024;
class  MsgNode {
public:
    MsgNode(const char* msg, int total_len) : _total_len(total_len), _cur_len(0) {
        _msg = new char[total_len]; 
        memcpy(_msg, msg, total_len);
    }//���ͽڵ�
    MsgNode(int total_len) :_total_len(total_len), _cur_len(0) {
        _msg = new char[total_len];
    }//���սڵ�
    ~MsgNode() {
        delete[]_msg;
    }
    //��Ϣ�׵�ַ
    char* _msg;
    //�ܳ���
    int _total_len;
    //��ǰ����
    int _cur_len;
};

class session
{
public:
	session(std::shared_ptr<asio::ip::tcp::socket> socket);
	void connect(asio::ip::tcp::endpoint &ep);
   
    void WriteCallBackErr(const boost::system::error_code& ec, std::size_t bytes_transferred,
        std::shared_ptr<MsgNode>);
    void WriteToSocketErr(const std::string& buf);
    
    void WriteCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void WriteToSocket(const std::string& buf);
    
    void WriteAllToSocket(const std::string& buf);
    void WriteAllCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
    
    void ReadCallBack(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void ReadFromSocket();
    //����һ��reciveallcallback recivefromsocket��������һ�ζ���APIʹ���÷���������ͬ
private:
	std::shared_ptr<asio::ip::tcp::socket> _socket;
	std::shared_ptr<MsgNode> _send_node;
    std::queue<std::shared_ptr<MsgNode>> _send_queue;
    std::shared_ptr<MsgNode> _recv_node;
    std::queue<std::shared_ptr<MsgNode>> _read_queue;
    bool _send_pending;
    bool _recv_pending;
};


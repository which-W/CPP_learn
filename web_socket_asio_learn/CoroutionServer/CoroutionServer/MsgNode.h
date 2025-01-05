#pragma once
#include<iostream>
#include"boost/asio.hpp"
#include"const.h"
class MsgNode
{
public:
	MsgNode(short Max_Length) :_total_len(Max_Length), _cur_len(0) {
		_data = new char[_total_len + 1]();
		_data[_total_len] = '\0';
	}
	~MsgNode() {
		std::cout << "destroy MsgNode" << std::endl;
		delete[] _data;
	}
	void Clear() {
		::memset(_data, 0, _total_len);
		_cur_len = 0;
	}

	short _cur_len;
	short _total_len;
	char* _data;
};

class RecvNode : public MsgNode
{
public:
	RecvNode(short max_len, short msg_id);
	short GetRecvID();
private:
	short _msg_id;
};

class SendNode : public MsgNode 
{
public:
	SendNode(const char * msg ,short max_len,short msg_id);
private:
	short _msg_id;
};
// beast-websocket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//



#include<iostream>
#include "WebSocketServer.h"
int main()
{
	net::io_context ioc;
	WebSocketServer server(ioc, 10086);
	server.StartAccept();
	ioc.run();

}


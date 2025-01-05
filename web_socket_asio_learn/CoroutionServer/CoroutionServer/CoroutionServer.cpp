// CoroutionServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <mutex>
#include <thread>
#include "AsioIOServicePool.h"
#include "CServer.h"
#include <csignal>
#include <boost/asio.hpp>
int main()
{
    try {
        auto& pool = AsioIOServicePool::GetInstance();
        boost::asio::io_context io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, &pool](auto, auto) {
            io_context.stop();
            pool.Stop();
            });
        CServer s(io_context, 10086);
        io_context.run();
    }
    catch (std::exception &ec) {
        std::cout << "Exeption is" << ec.what() << std::endl;
    }
    
    return 0;
}


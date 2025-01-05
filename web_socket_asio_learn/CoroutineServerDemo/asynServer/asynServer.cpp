// asynServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<boost/asio/co_spawn.hpp>
#include<boost/asio/detached.hpp>
#include<boost/asio/io_context.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio/signal_set.hpp>
#include<boost/asio/write.hpp>
using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

awaitable<void>echo(tcp::socket socket) {
    try {
        char data[1024];
        for (;;) {
            size_t n =co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
            co_await async_write(socket, boost::asio::buffer(data,n), use_awaitable);
        }
    }
    catch (std::exception & ec) {
        std::cout << "Exception is " << ec.what() << std::endl;
    }
}
awaitable<void> listener() {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, { tcp::v4(),10086 });
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        co_spawn(executor, echo(std::move(socket)), detached);
    }

}



int main()
{
    try {
        boost::asio::io_context io_context(1);
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) {
            io_context.stop();
            });
        co_spawn(io_context, listener(),detached); //co_spawn启动一个协程
        io_context.run();
    }
    catch(std::exception &e){
        std::cout << "Exception is" << e.what() << std::endl;
    }
}



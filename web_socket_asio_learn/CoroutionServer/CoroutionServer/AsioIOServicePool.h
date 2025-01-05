#pragma once
#include<boost/asio.hpp>
class AsioIOServicePool
{
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator = (const AsioIOServicePool&) = delete;
	~AsioIOServicePool();
	boost::asio::io_context& GetIOService();
	void Stop();
	static AsioIOServicePool& GetInstance();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _Works;
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;

};


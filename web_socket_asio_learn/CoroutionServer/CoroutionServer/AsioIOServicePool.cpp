#include "AsioIOServicePool.h"
#include<iostream>
using namespace std;

AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size), _Works(size), _nextIOService(0) {
	for (size_t i = 0; i < size; i++)
	{
		_Works[i] = std::unique_ptr<Work>(new Work(_ioServices[i]));
	}
	for (size_t i = 0; i < _ioServices.size(); i++)
	{
		_threads.emplace_back([this,i]() {
			_ioServices[i].run();
			});
	}
}

AsioIOServicePool& AsioIOServicePool::GetInstance() {
	static AsioIOServicePool instance(1);
	return instance;
}

AsioIOServicePool::~AsioIOServicePool() {
	std::cout << "AsioIOServicePool is destroy" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() {
	for (auto &work : _Works)
	{
		work.reset();
	}
	for (auto& t : _threads) {
		t.join();
	}
}
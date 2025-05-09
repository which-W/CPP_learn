#pragma once
#include <thread>
#include <vector>

class join_threads {


	std::vector<std::thread>& threads;
public:
	explicit join_threads(std::vector<std::thread>& _threads) :threads(_threads) {

	}

	~join_threads(){
		for (unsigned long i = 0; i < threads.size(); i++)
		{
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
	}
};
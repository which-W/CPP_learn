// thread15_red_queue.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "lockfreequeue.h"
#include <thread>
#define TESTCOUNT 1000
void TestLockFreeQue() {
	lock_free_queue<int>  que;
	std::thread t1([&]() {
		for (int i = 0; i < TESTCOUNT; i++) {
			que.push(i);
			std::cout << "push data is " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		});



	std::thread t2([&]() {
		for (int i = 0; i < TESTCOUNT;) {
			auto p = que.pop();
			if (p == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			i++;
			std::cout << "pop data is " << *p << std::endl;
		}
		});

	t1.join();
	t2.join();

	assert(que.destruct_count == TESTCOUNT);
}
int main()
{
	TestLockFreeQue();
}


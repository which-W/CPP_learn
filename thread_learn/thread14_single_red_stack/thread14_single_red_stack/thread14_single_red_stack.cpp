// thread14_single_red_stack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "single_ref_stack.h"
#include "Refstack.h"
#include <set>
#include <thread>
#include <mutex>
#include <cassert>
void TestSingleRefStack() {
	single_ref_stack<int>  single_ref_stack;
	std::set<int>  rmv_set;
	std::mutex set_mtx;

	std::thread t1([&]() {
		for (int i = 0; i < 20000; i++) {
			single_ref_stack.push(i);
			std::cout << "push data " << i << " success!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		});

	std::thread t2([&]() {
		for (int i = 0; i < 10000;) {
			auto head = single_ref_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < 10000;) {
			auto head = single_ref_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	t1.join();
	t2.join();
	t3.join();

	assert(rmv_set.size() == 20000);
}

void TestRefCountStack() {
	ref_count_stack<int>  ref_count_stack;
	std::set<int>  rmv_set;
	std::mutex set_mtx;

	std::thread t1([&]() {
		for (int i = 0; i < 20000; i++) {
			ref_count_stack.push(i);
			std::cout << "push data " << i << " success!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		});

	std::thread t2([&]() {
		for (int i = 0; i < 10000;) {
			auto head = ref_count_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	std::thread t3([&]() {
		for (int i = 0; i < 10000;) {
			auto head = ref_count_stack.pop();
			if (!head) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			std::lock_guard<std::mutex> lock(set_mtx);
			rmv_set.insert(*head);
			std::cout << "pop data " << *head << " success!" << std::endl;
			i++;
		}
		});

	t1.join();
	t2.join();
	t3.join();

	assert(rmv_set.size() == 20000);
}
int main()
{
	//TestSingleRefStack();
	TestRefCountStack();
}


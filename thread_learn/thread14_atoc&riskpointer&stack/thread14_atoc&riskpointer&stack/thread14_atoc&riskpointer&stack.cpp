// thread14_atoc&riskpointer&stack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <thread>
#include <set>
#include <mutex>
#include <cassert>
#include "HazardPointerStack.h"
hazard_pointer hazard_pointers[max_hazard_pointers];
void TestHazardPointer() {
    hazard_pointer_stack<int> hazard_stack;
    std::set<int>  rmv_set;
    std::mutex set_mtx;

    std::thread t1([&]() {
        for (int i = 0; i < 20000; i++) {
            hazard_stack.push(i);
            std::cout << "push data " << i << " success!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        });

    std::thread t2([&]() {
        for (int i = 0; i < 10000;) {
            auto head = hazard_stack.pop();
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
            auto head = hazard_stack.pop();
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
    TestHazardPointer();
}


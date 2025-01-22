// thread18_thread_pool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "simple_thread_pool.h"
#include <iostream>
#include "parallenForeach.h"
void TestSimpleThread() {
	std::vector<int> nvec;
	for (int i = 0; i < 26; i++) {
		nvec.push_back(i);
	}

	simple_for_each(nvec.begin(), nvec.end(), [](int& i) {
		i *= i;
		});

	for (int i = 0; i < nvec.size(); i++) {
		std::cout << nvec[i] << " ";
	}

	std::cout << std::endl;
}

int main()
{
    std::cout << "Hello World!\n";
}


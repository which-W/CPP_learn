// thread13_safe_list.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <set>
#include"threadsafelist.h"
#include "MyClass.h"

std::set<int> removeSet;
void TestThreadSafeList()
{

	threadsafe_list<MyClass> thread_safe_list;
	std::thread t1([&]()
		{
			for (unsigned int i = 0; i < 100; i++)
			{
				MyClass mc(i);
				thread_safe_list.push_front(mc);
			}

		});


	std::thread t2([&]()
		{
			for (unsigned int i = 0; i < 100; )
			{

				auto find_res = thread_safe_list.find_first_if([&](auto& mc)
					{
						return mc.GetData() == i;
					});

				if (find_res == nullptr)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}

				removeSet.insert(i);
				i++;
			}
		});

	t1.join();
	t2.join();

}
int main()
{
	TestThreadSafeList();
	for (auto& v : removeSet) {
		std::cout << "removeset data is " << v << std::endl;
	}
}


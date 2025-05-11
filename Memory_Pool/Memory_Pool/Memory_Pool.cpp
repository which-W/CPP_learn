// Memory_Pool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Memory_Base_Pool.h"
#include "Students.h"
int main()
{
	{ //RAII封装
		try
		{
			Memory_Base_Pool pool(sizeof(Students), 3); //创建一个内存池，大小为3个Students对象

			void* ptr1 = pool.Allocate(); // 分配内存
			void* ptr2 = pool.Allocate(); // 分配内存
			void* ptr3 = pool.Allocate(); // 分配内存

			Students* student1 = new (ptr1) Students("Alice", 20); // 在分配的内存上构造对象
			Students* student2 = new (ptr2) Students("Bob", 22); // 在分配的内存上构造对象
			Students* student3 = new (ptr3) Students("Charlie", 21); // 在分配的内存上构造对象

			std::cout << "student 1: " << student1->name << ", " << student1->age << std::endl;
			std::cout << "student 2: " << student2->name << ", " << student2->age << std::endl;
			std::cout << "student 3: " << student3->name << ", " << student3->age << std::endl;

			//使用完析构students
			student1->~Students();
			student2->~Students();
			student3->~Students();

			// 回收内存
			pool.Deallocate(ptr1);
			pool.Deallocate(ptr2);
			pool.Deallocate(ptr3);
		}
		catch (const std::bad_alloc& ec)
		{
			std::cout << "Memory allocate error :" << ec.what() << std::endl;
			return -1;
		}
	}
}


#pragma once
#include<stack>
class Memory_Base_Pool
{
public:
	Memory_Base_Pool(size_t object_size, size_t pool_size);
	~Memory_Base_Pool();
	void* Allocate(); // 分配内存
	void Deallocate(void* ptr); // 回收内存
private:
	size_t object_size; // 每个对象的大小
	size_t pool_size; // 内存池的大小
	char* pool; // 内存池的指针
	std::stack<void*> free_list; // 空闲对象列表
};


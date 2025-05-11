#include "Memory_Base_Pool.h"

Memory_Base_Pool::Memory_Base_Pool(size_t object_size, size_t pool_size):object_size(object_size),pool_size(pool_size),pool
((char*)malloc(object_size* pool_size)), free_list(std::stack<void*>())
{
	if (pool == nullptr)
	{
		throw std::bad_alloc();
	}
	for (size_t i = 0; i < pool_size; ++i)
	{
		free_list.push(pool + i * object_size);
	}
}

Memory_Base_Pool::~Memory_Base_Pool()
{
	if (pool != nullptr)
	{
		free(pool);
	}
}

void* Memory_Base_Pool::Allocate()
{
	if (free_list.empty())
	{
		return nullptr;
	}
	void* ptr = free_list.top();
	free_list.pop();
	return ptr;
}

void Memory_Base_Pool::Deallocate(void* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}
	free_list.push((char*)ptr);
}

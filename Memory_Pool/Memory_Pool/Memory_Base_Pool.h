#pragma once
#include<stack>
class Memory_Base_Pool
{
public:
	Memory_Base_Pool(size_t object_size, size_t pool_size);
	~Memory_Base_Pool();
	void* Allocate(); // �����ڴ�
	void Deallocate(void* ptr); // �����ڴ�
private:
	size_t object_size; // ÿ������Ĵ�С
	size_t pool_size; // �ڴ�صĴ�С
	char* pool; // �ڴ�ص�ָ��
	std::stack<void*> free_list; // ���ж����б�
};


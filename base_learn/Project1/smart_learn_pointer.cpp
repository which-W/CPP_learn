#include<iostream>
#include<vld.h>
#include<memory>

int main() {
	//可以用new来初始化智能指针
	std::shared_ptr	<int> shared_ptr(new int(100));
	//创建数组只能用New,对数组的处理智能指针做的并不好
	std::shared_ptr	<int> shared_ptr(new int[100]());
	//make_shared更为规范以后都用这个初始化
	std::shared_ptr	<int> shared_LD = std::make_shared<int>(100);
	std::shared_ptr	<int> shared_LD = std::make_shared<int>(100);
	//也可以用复制构架函数来初始化
	std::shared_ptr	<int> shared_LD2(shared_LD);
	//shared_LD.use_count()可以查出智能指针的引用计数而reset()可以使得该对象内存的指针设置为Null。
	//shared_LD.swap(shared_LD2)可以让两个指针指向的内存地址互换。

	//weak_ptr只引用地址而不增加引用计数,当两个堆内存互相指向则需要Weak来解决循环引用
	std::weak_ptr<int> weakid(shared_LD);

	//unque_ptr是对独占内存的智能指针，其初始化和Share_ptr完全相同
	std::unique_ptr<int>uniqueid(new int(100));
	std::unique_ptr<int>uniqueid = std::make_unique<int>(100);
	


	//智能指针在处理数组的时候处理的并不好，很多情况下在类中实现数组只能用普通指针。
	return 0;
}

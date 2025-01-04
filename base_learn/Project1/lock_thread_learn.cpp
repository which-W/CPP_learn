#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
//使用锁来实现子线程和其他线程之间同时占有适配器而产生的数据错误
//有mymutex的lock但容易出先忘记unlock或者因为程序而出现Unlock未到运行而线程结束进而导致出现问题
//所以一般使用lock_guard来作为锁
//也可以用原子操作使得数据不可分割而进而达到同时只有一个线程使用数据
//如： std::atomic<unsigned>ans = 0;
//注意锁会极大影响运行效率所以必须加在必要的数据区域之类。
unsigned  ans = 0;
std::mutex mymutex;
void test() {
	
	//当{}中的内容结束会调用lg的析构函数进而调用Mymutex的unlock函数
	
	for (int i; i < 20000; i++) {
		std::lock_guard<std::mutex>lg(mymutex);
		ans++;
	}
}
int main() {
	std::thread thr(test);
	for (int i; i < 20000; i++) {
		std::lock_guard<std::mutex>lg(mymutex);
		ans++;
	}
	thr.join();
	std::cout << ans << std::endl;
	return 0;
}
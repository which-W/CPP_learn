#include<iostream>
#include<thread>


/*一个程序运行时就是打开一个进程，其中的main函数是一个主线程，其他的线程被称为子线程。当主线程执行完毕后就会
关闭进程子线程的资源会被剥夺*/
/*
可以给子线程传参，但是不能传递指针和隐式对象传递，因为这样可能导致主线程已经执行完了
子线程还没有执行完而导致的参数出错。
*/
class Test
{
public:
	Test(int i_) :i(i_)
	{
		std::cout << i << std::endl;
	}
	Test(const Test& test) :i(test.i) {

	}
	int i =10;
};
/*
	类类型传递引用就可以了，类类型传递引用会首先调用一次复制构造函数生成一个临时变量，
	故而导致地址不相同。如果采用值传递，需要两次复制构造函数，开销更大。
*/
void test3( Test & i) {
	i = 200;
}

void test1(int i, int& i1) {

}
int main() {
	std::thread thr([]() {
		for (int i = 0; i < 10000; i++) {

		}
		});
	/*
	普通类型在传递子线程函数参数时，直接值传递即可。
	*/
	int i = 10;
	std::thread thr1(test1, i, i);
	
	Test t(10);
	//当使用std::ref时就不会调用复制构造函数，这样可以使得函数可以修改类内部的值将i变为200。
	std::thread thr1(test3, std::ref(t));
	//当调用join函数时主线程会等待子线程执行完毕后再继续执行，需要等待运用这个
	thr.join();
	//当调用detach函数时子线程会交给C++代码执行库来执行，这样即使主线程执行完毕进程关闭也不
	// 会影响子线程的执行，不需要等待运用
	thr.detach();
		return 0;
}
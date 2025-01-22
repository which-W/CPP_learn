// thread04_single&mutex.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
//用局部静态变量来创建，因为其只会创建一次只有第一次函数调用时创建之后便不再创建，不过这个方式
//只有在c++11以及以后安全
class Single2 {
private:
    Single2()
    {
    }
    Single2(const Single2&) = delete;
    Single2& operator=(const Single2&) = delete;
public:
    static Single2& GetInst()
    {
        static Single2 single;
        return single;
    }
};


//饿汉式
//在主线程初始化来避免其他线程再次初始化
class Single2Hungry
{
private:
    Single2Hungry()
    {
    }
    Single2Hungry(const Single2Hungry&) = delete;
    Single2Hungry& operator=(const Single2Hungry&) = delete;
public:
    static Single2Hungry* GetInst()
    {
        if (single == nullptr)
        {
            single = new Single2Hungry();
        }
        return single;
    }
private:
    static Single2Hungry* single;
};


//饿汉式初始化
//饿汉式是从使用角度规避多线程的安全问题，很多情况下我们很难从规则角度限制开发人员，
// 所以这种方式不是很推荐。
Single2Hungry* Single2Hungry::single = Single2Hungry::GetInst();
void thread_func_s2(int i)
{
    std::cout << "this is thread " << i << std::endl;
    std::cout << "inst is " << Single2Hungry::GetInst() << std::endl;
}
void test_single2hungry()
{
    std::cout << "s1 addr is " << Single2Hungry::GetInst() << std::endl;
    std::cout << "s2 addr is " << Single2Hungry::GetInst() << std::endl;
    for (int i = 0; i < 3; i++)
    {
        std::thread tid(thread_func_s2, i);
        tid.join();
    }
}


//懒汉式
//由于在线程中开辟堆空间所有需要释放，但因为多线程情况下无法确定是哪个线程开辟的原因而无法析构
class SinglePointer
{
private:
    SinglePointer()
    {
    }
    SinglePointer(const SinglePointer&) = delete;
    SinglePointer& operator=(const SinglePointer&) = delete;
public:
    static SinglePointer* GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = new SinglePointer();
        s_mutex.unlock();
        return single;
    }
private:
    static SinglePointer* single;
    static std::mutex s_mutex;
};

//懒汉式调用
SinglePointer* SinglePointer::single = nullptr;
std::mutex SinglePointer::s_mutex;
void thread_func_lazy(int i)
{
    std::cout << "this is lazy thread " << i << std::endl;
    std::cout << "inst is " << SinglePointer::GetInst() << std::endl;
}
void test_singlelazy()
{
    for (int i = 0; i < 3; i++)
    {
        std::thread tid(thread_func_lazy, i);
        tid.join();
    }
    //何时释放new的对象？造成内存泄漏
}


//智能指针懒汉式
//可以利用智能指针完成自动回收

//为了规避用户手动释放内存，可以提供一个辅助类帮忙回收内存
//并将单例类的析构函数写为私有
class SingleAutoSafe;
class SafeDeletor
{
public:
    void operator()(SingleAutoSafe* sf)
    {
        std::cout << "this is safe deleter operator()" << std::endl;
        delete sf;
    }
};

class SingleAutoSafe
{
private:
    SingleAutoSafe()
    {
    }
    SingleAutoSafe(const SingleAutoSafe&) = delete;
    SingleAutoSafe& operator=(const SingleAutoSafe&) = delete;
    ~SingleAutoSafe()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    friend class SafeDeletor;
public:
   
    static std::shared_ptr<SingleAutoSafe> GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = std::shared_ptr<SingleAutoSafe>(new SingleAutoSafe, SafeDeletor());
        s_mutex.unlock();
        return single;
    }
private:
    static std::shared_ptr<SingleAutoSafe> single;
    static std::mutex s_mutex;
};

//但是上面的代码存在危险，比如懒汉式的使用方式，当多个线程调用单例时，有一个线程加锁进入3处的逻辑。
//其他的线程有的在1处，判断指针非空则跳过初始化直接使用单例的内存会存在问题。
//主要原因在于SingleAutoSafe* temp = new SingleAutoSafe() 这个操作是由三部分组成的
//1 调用allocate开辟内存
//2 调用construct执行SingleAutoSafe的构造函数
//3 调用赋值操作将地址赋值给temp
//
//而现实中2和3的步骤可能颠倒，所以有可能在一些编译器中通过优化是1，3，2的调用顺序，
//其他线程取到的指针就是非空，还没来的及调用构造函数就交给外部使用造成不可预知错误。
//为解决这个问题，C++11 推出了std::call_once函数保证多个线程只执行一次

//真正安全的智能指针类型的单例模式
template <typename T>
class Singleton {
protected:
    Singleton() = default;0.
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>& st) = delete;
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        //类似于内部有锁  只会让一个线程进入初始化即使多个函数都判断s_flag为false
        std::call_once(s_flag, [&]() {
            _instance = std::shared_ptr<T>(new T);
            });
        return _instance;
    }
    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;


//如果只是实现一个简单的单例类推荐使用返回局部静态变量的方式
//如果想大规模实现多个单例类可以用call_once实现的模板类。
int main()
{
    std::cout << "Hello World!\n";
}



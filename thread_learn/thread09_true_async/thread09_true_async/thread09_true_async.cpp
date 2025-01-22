// thread09_true_async.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <future>


//使用async要注意其返回的future是不是shared_state的最后持有者。
//这句话的意思是如果创建异步async那么不应该将future的持有与使用都在一个函数内或者
// 通过局部作用域析构future而任务却没有执行完实现
// 这样会因为阻塞而导致串行
//例如
void BlockAsync() {
    std::cout << "begin block async" << std::endl;
    {
        std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "std::async called " << std::endl;
            });
    }
    std::cout << "end block async" << std::endl;
}
//这样使用才能实现异步
int asyncFunc() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "this is asyncFunc" << std::endl;
    return 0;
}
void func1(std::future<int>& future_ref) {
    std::cout << "this is func1" << std::endl;
    future_ref = std::async(std::launch::async, asyncFunc);
}
void func2(std::future<int>& future_ref) {
    std::cout << "this is func2" << std::endl;
    auto future_res = future_ref.get();
    if (future_res == 0) {
        std::cout << "get asyncFunc result success !" << std::endl;
    }
    else {
        std::cout << "get asyncFunc result failed !" << std::endl;
        return;
    }
}
//提供多种思路，这是第一种
void first_method() {
    std::future<int> future_tmp;
    func1(future_tmp);
    func2(future_tmp);
}

//可以直接使用的完全异步
template<typename Func, typename... Args  >
auto  ParallenExe(Func&& func, Args && ... args) -> std::future<decltype(func(args...))> {
    typedef    decltype(func(args...))  RetType;
    std::function<RetType()>  bind_func = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    std::packaged_task<RetType()> task(bind_func);
    auto rt_future = task.get_future();
    std::thread t(std::move(task));
    t.detach();
    return rt_future;
}
//测试
void TestParallen1() {
    int i = 0;
    std::cout << "Begin TestParallen1 ..." << std::endl;
    {
        ParallenExe([](int i) {
            while (i < 3) {
                i++;
                std::cout << "ParllenExe thread func " << i << " times" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            }, i);
    }
    std::cout << "End TestParallen1 ..." << std::endl;
}
//更合理的使用，回合Future而不需要考虑主线程的结束
void TestParallen2() {
    int i = 0;
    std::cout << "Begin TestParallen2 ..." << std::endl;
    auto rt_future = ParallenExe([](int i) {
        while (i < 3) {
            i++;
            std::cout << "ParllenExe thread func " << i << " times" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, i);
    std::cout << "End TestParallen2 ..." << std::endl;
    //等待future的任务的结束
    rt_future.wait();
}
int main()
{
    /*TestParallen1();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Main Exited!\n";*/
    TestParallen2();
}


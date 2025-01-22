// thread_01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <sstream>
#include <chrono>
#include <vector>
void thread_work1(std::string str) {
    std::cout << "str is" << str <<std::endl;
}

struct func {
    int& _i;
    func(int& i) : _i(i) {}
    void operator()() {
        for (int i = 0; i < 3; i++) {
            _i = i;
            std::cout << "_i is " << _i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

class thread_guard { //守护线程
private:
    std::thread& _t;
public:
    explicit thread_guard(std::thread& t) :_t(t) {}
    ~thread_guard() {
        //join只能调用一次
        if (_t.joinable()) {
            _t.join();
        }
    }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
};

class joining_thread { //比较完善的守护线程
    std::thread  _t;
public:
    joining_thread() noexcept = default;
    template<typename Callable, typename ...  Args>
    explicit  joining_thread(Callable&& func, Args&& ...args) :
        t(std::forward<Callable>(func), std::forward<Args>(args)...) {}
    explicit joining_thread(std::thread  t) noexcept : _t(std::move(t)) {}
    joining_thread(joining_thread&& other) noexcept : _t(std::move(other._t)) {}
    joining_thread& operator=(joining_thread&& other) noexcept
    {
        //如果当前线程可汇合，则汇合等待线程完成再赋值
        if (joinable()) {
            join();
        }
        _t = std::move(other._t);
        return *this;
    }
    joining_thread& operator=(joining_thread other) noexcept
    {
        //如果当前线程可汇合，则汇合等待线程完成再赋值
        if (joinable()) {
            join();
        }
        _t = std::move(other._t);
        return *this;
    }
    ~joining_thread() noexcept {
        if (joinable()) {
            join();
        }
    }
    void swap(joining_thread& other) noexcept {
        _t.swap(other._t);
    }
    std::thread::id   get_id() const noexcept {
        return _t.get_id();
    }
    bool joinable() const noexcept {
        return _t.joinable();
    }
    void join() {
        _t.join();
    }
    void detach() {
        _t.detach();
    }
    std::thread& as_thread() noexcept {
        return _t;
    }
    const std::thread& as_thread() const noexcept {
        return _t;
    }
};

void use_jointhread() {
    //1 根据线程构造函数构造joiningthread
    joining_thread j1([](int maxindex) {
        for (int i = 0; i < maxindex; i++) {
            std::cout << "in thread id " << std::this_thread::get_id()
                << " cur index is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, 10);

    //2 根据thread构造joiningthread
    joining_thread j2(std::thread([](int maxindex) {
        for (int i = 0; i < maxindex; i++) {
            std::cout << "in thread id " << std::this_thread::get_id()
                << " cur index is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, 10));

    //3 根据thread构造j3
    joining_thread j3(std::thread([](int maxindex) {
        for (int i = 0; i < maxindex; i++) {
            std::cout << "in thread id " << std::this_thread::get_id()
                << " cur index is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, 10));


    //4 把j3赋值给j1，joining_thread内部会等待j1汇合结束后
    //再将j3赋值给j1
    j1 = std::move(j3);

}

void param_function(int a) {
    while (true) {
        std::cout << "param is " << a << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void use_vector() {
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 10; ++i) {
        //不要使用push_back来操作新的线程加入，如果使用则需要这样写,这样写会拷贝两次不推荐
       /* auto t = std::thread(param_function, i);
        threads.push_back(std::move(t));*/
        threads.emplace_back(param_function, i);
    }
    for (auto& entry : threads) {
        entry.join();
    }
}

void oops() {
    int some_local_state = 0;
    func myfunc(some_local_state);
    std::thread functhread(myfunc);
    //隐患，访问局部变量，局部变量可能会随着}结束而回收或随着主线程退出而回收
    functhread.detach();
}

void use_join() {
    int some_local_state = 0;
    func myfunc(some_local_state);
    std::thread functhread(myfunc);
    //这个线程会阻塞让下面的函数需要等待
    functhread.join();
}

void auto_guard() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread  t(my_func);
    thread_guard g(t);
    //本线程做一些事情
    std::cout << "auto guard finished " << std::endl;
}

void print_str(int i, std::string const& s) {
    std::cout << "i is " << i << " str is " << s << std::endl;
}

void safe_oops(int some_param) {
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    //当线程要调用的回调函数参数为引用类型时，需要将参数显示转化为引用对象传递给线程的构造函数，
    std::thread t(print_str, 3, std::string(buffer));
    t.detach();
}

//重要!!!!!!
void change_param(int& param) {
    param++;
}
void ref_oops(int some_param) {
    std::cout << "before change , param is " << some_param << std::endl;
    //需使用引用显示转换
    std::thread  t2(change_param, std::ref(some_param));
    t2.join();
    std::cout << "after change , param is " << some_param << std::endl;
}
void catch_exception() {
    int some_local_state = 0;
    func myfunc(some_local_state);
    std::thread  functhread{ myfunc };
    try {
        //本线程做一些事情,可能引发崩溃
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    catch (std::exception& e) {
        //主线程崩溃了依旧需要等待子线程的完成
        functhread.join();
        throw;
    }
    functhread.join();
}

class background_task {
public:
    void operator()() {
        std::cout << "background_task called" << std::endl;
    }
};

class X
{
public:
    void do_lengthy_work() {
        std::cout << "do_lengthy_work " << std::endl;
    }
};
void bind_class_oops() {
    X my_x;
    //传入函数地址与类的地址，函数地址加不加&都可以取到函数地址
    std::thread t(&X::do_lengthy_work, &my_x);
    t.join();
}
//有时候传递给线程的参数是独占的，所谓独占就是不支持拷贝赋值和构造，
// 但是我们可以通过std::move的方式将参数的所有权转移给线程
void deal_unique(std::unique_ptr<int> p) {
    std::cout << "unique ptr data is " << *p << std::endl;
    (*p)++;
    std::cout << "after unique ptr data is " << *p << std::endl;
}
void move_oops() {
    auto p = std::make_unique<int>(100);
    std::thread  t(deal_unique, std::move(p));
    t.join();
    //不能再使用p了，p已经被move废弃
   // std::cout << "after unique ptr data is " << *p << std::endl;
}


void some_other_function() {
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
int main()
{
   std::string str = "hello world";

   //thread绑定的回调函数是普通函数，可以在函数前加&或者不加&，
   // 因为编译器默认将普通函数名作为函数地址，如下两种写法都正确
   std::thread t1(thread_work1, str);
   std::thread t11(&thread_work1, str);
   std::thread t2{ background_task() };

   std::thread t3([](std::string str) {
       std::cout << "str is " << str << std::endl;
       }, str);


   // detach 注意事项
   oops();
   //防止主线程退出过快，需要停顿一下，让子线程跑起来detach
   std::this_thread::sleep_for(std::chrono::seconds(1));

   //thread不存在拷贝复制只能使用移动，t4无效但可以继续使用
   std::thread t4(thread_work1, str);
   std::thread t5 = std::move(t4);
   //返回的是一个局部变量是一个右值
   t4 = std::thread(some_other_function);
   //捕获异常
   catch_exception();

   //守护线程
   auto_guard();
   //2 主线程等待子线程退出
   // join 用法
   use_join();
   t1.join();
   t2.join();
   t3.join();
}


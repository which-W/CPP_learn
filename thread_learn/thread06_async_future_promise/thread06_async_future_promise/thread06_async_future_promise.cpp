// thread06_async_future_promise.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
// 定义一个异步任务
std::string fetchDataFromDB(std::string query) {
    // 模拟一个异步任务，比如从数据库中获取数据
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return "Data: " + query;
}

//std::future的wait_for()或wait_until()方法来检查异步操作是否已完成。
// 这些方法返回一个表示操作状态的std::future_status值。
//if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
//    // 操作已完成  
//}
//else {
//    // 操作尚未完成  
//}


//创建一个std::packaged_task对象，该对象包装了要执行的任务。
//调用std::packaged_task对象的get_future()方法，该方法返回一个与任务关联的std::future对象。
//在另一个线程上调用std::packaged_task对象的operator()，以执行任务。
//在需要任务结果的地方，调用与任务关联的std::future对象的get()方法，以获取任务的返回值或异常。
int my_task() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "my task run 5 s" << std::endl;
    return 42;
}
void use_package() {
    // 创建一个包装了任务的 std::packaged_task 对象  
    std::packaged_task<int()> task(my_task);
    // 获取与任务关联的 std::future 对象  
    std::future<int> result = task.get_future();
    // 在另一个线程上执行任务  
    std::thread t(std::move(task));
    t.detach(); // 将线程与主线程分离，以便主线程可以等待任务完成  
    // 等待任务完成并获取结果  
    int value = result.get();
    std::cout << "The result is: " << value << std::endl;
}


//std::promise用于在某一线程中设置某个值或异常，而std::future则用于在另一线程中获取这个值或异常。
//promise用法
void set_value(std::promise<int> prom) {
    // 设置 promise 的值
    prom.set_value(10);
}
//promise的异常函数
void set_exception(std::promise<void> prom) {
    try {
        // 抛出一个异常
        throw std::runtime_error("An error occurred!");
    }
    catch (...) {
        // 设置 promise 的异常
        prom.set_exception(std::current_exception());
    }
}
void use_promise_exception() {
    std::promise<void> prom;
    // 获取与 promise 相关联的 future 对象
    std::future<void> fut = prom.get_future();
    // 在新线程中设置 promise 的异常
    std::thread t(set_exception, std::move(prom));
    // 在主线程中获取 future 的异常
    try {
        std::cout << "Waiting for the thread to set the exception...\n";
        fut.get();
    }
    catch (const std::exception& e) {
        std::cout << "Exception set by the thread: " << e.what() << '\n';
    }
    t.join();
}

//promise和shared_future
void myFunction(std::promise<int>&& promise) {
    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    promise.set_value(42); // 设置 promise 的值
}
void threadFunction(std::shared_future<int> future) {
    try {
        int result = future.get();
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::future_error& e) {
        std::cout << "Future error: " << e.what() << std::endl;
    }
}
void use_shared_future() {
    std::promise<int> promise;
    //隐式转换 将future自动转换为shared_future
    std::shared_future<int> future = promise.get_future();
    std::thread myThread1(myFunction, std::move(promise)); // 将 promise 移动到线程中
    // 使用 share() 方法获取新的 shared_future 对象  
    std::thread myThread2(threadFunction, future);
    std::thread myThread3(threadFunction, future);
    myThread1.join();
    myThread2.join();
    myThread3.join();
}

//鲁棒线程池
//不能使用线程池的情况
//1，如果需要有序处理
//2，任务之间存在强关联性
class ThreadPool {
public:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    static ThreadPool& instance() {
        static ThreadPool ins;
        return ins;
    }
    using Task = std::packaged_task<void()>;
    ~ThreadPool() {
        stop();
    }
    template <class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...)); //decltype(f(args...))这个是在推导函数的返回类型
        if (stop_.load())
            return std::future<RetType>{};
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        //使用bind返回一个无参函数返回值类型为RetType
        std::future<RetType> ret = task->get_future();
        {
            std::lock_guard<std::mutex> cv_mt(cv_mt_);
            tasks_.emplace([task] { (*task)(); });
        }
        cv_lock_.notify_one();
        return ret;
    }
    int idleThreadCount() {
        return thread_num_;
    }
private:
    ThreadPool(unsigned int num = 5)
        : stop_(false) {
            {
                if (num < 1)
                    thread_num_ = 1;
                else
                    thread_num_ = num;
            }
            start();
    }
    void start() {
        for (int i = 0; i < thread_num_; ++i) {
            pool_.emplace_back([this]() {
                while (!this->stop_.load()) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> cv_mt(cv_mt_);
                        this->cv_lock_.wait(cv_mt, [this] {
                            return this->stop_.load() || !this->tasks_.empty();
                            });
                        if (this->tasks_.empty())
                            return;
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    this->thread_num_--;
                    task();
                    this->thread_num_++;
                }
                });
        }
    }
    void stop() {
        stop_.store(true);
        cv_lock_.notify_all();
        for (auto& td : pool_) {
            if (td.joinable()) {
                std::cout << "join thread " << td.get_id() << std::endl;
                td.join();
            }
        }
    }
private:
    std::mutex               cv_mt_;
    std::condition_variable  cv_lock_;
    std::atomic_bool         stop_;
    std::atomic_int          thread_num_;
    std::queue<Task>         tasks_;
    std::vector<std::thread> pool_;
};

int main() {
    //future使用
    // 使用 std::async 异步调用 fetchDataFromDB
   // std::launch::deferred：这种策略意味着任务将在调用std::future::get()或std::future::wait()函数时延迟执行。
   // 换句话说，任务将在需要结果时同步执行。
   // std::launch::async | std::launch::deferred：这种策略是上面两个策略的组合。任务可以在一个单独的线程上异步执行，也可以延迟执行，具体取决于实现。
    std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
    // 在主线程中做其他事情
    std::cout << "Doing something else..." << std::endl;
    // 从 future 对象中获取数据,
    // 这个get是一种阻塞get在子线程完成函数之后才能返回,不会阻塞主线程
    // std::future::get() 是一个阻塞调用，用于获取 std::future 对象表示的值或异常。
    // 如果异步任务还没有完成，get() 会阻塞当前线程，直到任务完成。
    // 如果任务已经完成，get() 会立即返回任务的结果。
    // 重要的是，get() 只能调用一次，因为它会移动或消耗掉 std::future 对象的状态。
    // 一旦 get() 被调用，std::future 对象就不能再被用来获取结果。
    std::string dbData = resultFromDB.get();
    std::cout << dbData << std::endl;
 



    //promise使用
    // 创建一个 promise 对象
    std::promise<int> prom;
    // 获取与 promise 相关联的 future 对象
    std::future<int> fut = prom.get_future();
    // 在新线程中设置 promise 的值
    std::thread t(set_value, std::move(prom));
    // 在主线程中获取 future 的值
    std::cout << "Waiting for the thread to set the value...\n";
    std::cout << "Value set by the thread: " << fut.get() << '\n';
    t.join();
    return 0;
}





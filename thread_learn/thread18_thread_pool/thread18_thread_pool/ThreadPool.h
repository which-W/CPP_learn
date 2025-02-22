#pragma once
#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


//比较健全的线程池
class NoneCopy {

public:
    ~NoneCopy() {}
protected:
    NoneCopy() {}
private:
    NoneCopy(const NoneCopy&) = delete;
    NoneCopy& operator=(const NoneCopy&) = delete;
};

//通过ThreadPool::instance().commit(传入函数与参数)来使用
class ThreadPool : public NoneCopy {
public:
    //继承基类NoneCopy就不需要写如下删除了
    //ThreadPool(const ThreadPool&) = delete;
    //ThreadPool& operator=(const ThreadPool&) = delete;

    //单例
    static ThreadPool& instance() {
        static ThreadPool ins;
        return ins;
    }
    //通用task
    using Task = std::packaged_task<void()>;


    ~ThreadPool() {
        stop();
    }

    //创建任务加入队列类似于生产者
    template <class F, class... Args>
    auto commit(F&& f, Args&&... args) ->
        std::future<decltype(std::forward<F>(f)(std::forward<Args>(args)...))> {
        using RetType = decltype(std::forward<F>(f)(std::forward<Args>(args)...));
        if (stop_.load())
            return std::future<RetType>{};

        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

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
    ThreadPool(unsigned int num = std::thread::hardware_concurrency())
        : stop_(false) {
            {
                if (num <= 1)
                    thread_num_ = 2;
                else
                    thread_num_ = num;
            }
            start();
    }
    //创建线程并执行任务类似于消费者
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

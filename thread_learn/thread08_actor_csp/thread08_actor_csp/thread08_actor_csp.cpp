// thread08_actor_csp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
template <typename T>
class Channel {
private:
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_producer_;
    std::condition_variable cv_consumer_;
    size_t capacity_;
    bool closed_ = false;
public:
    Channel(size_t capacity = 0) : capacity_(capacity) {}
    //生产
    bool send(T value) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_producer_.wait(lock, [this]() {
            // 对于无缓冲的channel，我们应该等待直到有消费者准备好
            return (capacity_ == 0 && queue_.empty()) || queue_.size() < capacity_ || closed_;
            });
        if (closed_) {
            return false;
        }
        queue_.push(value);
        cv_consumer_.notify_one();
        return true;
    }
    //消费
    bool receive(T& value) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_consumer_.wait(lock, [this]() { return !queue_.empty() || closed_; });
        if (closed_ && queue_.empty()) {
            return false;
        }
        value = queue_.front();
        queue_.pop();
        cv_producer_.notify_one();
        return true;
    }
    void close() {
        std::unique_lock<std::mutex> lock(mtx_);
        closed_ = true;
        cv_producer_.notify_all();
        cv_consumer_.notify_all();
    }
};
// 示例使用
int main() {
    Channel<int> ch(10);  // 10缓冲的channel
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            ch.send(i);
            std::cout << "Sent: " << i << std::endl;
        }
        ch.close();
        });
    std::thread consumer([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 故意延迟消费者开始消费
        int val;
        while (ch.receive(val)) {
            std::cout << "Received: " << val << std::endl;
        }
        });
    producer.join();
    consumer.join();
    return 0;
}


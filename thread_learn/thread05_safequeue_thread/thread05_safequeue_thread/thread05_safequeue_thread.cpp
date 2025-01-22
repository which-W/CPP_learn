// thread05_safequeue_thread.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>


int num = 1;
std::mutex mtx_num;
std::condition_variable cv1;
std::condition_variable cv2;

void Use_condition_lmplemention() {
    std::thread t1([]() {
        for (;;) {

            std::unique_lock<std::mutex> lock(mtx_num);
            //使用条件变量阻塞线程
            //第一种写法
          /*  while (num != 1) {
                cv1.wait(lock);
            }*/
           //第二种写法
            cv1.wait(lock, []() {
                return num == 1;
                });

            num++;
            std::cout << "thread A print 1...." << std::endl;
            cv2.notify_one();
            }
        });


    std::thread t2([]() {
        for (;;) {
            std::unique_lock<std::mutex> lock(mtx_num);
            cv2.wait(lock, []() {
                return num == 2;
                });
            num--;
            std::cout << "thread B print 2....." << std::endl;
            cv1.notify_one();
        }
        });
    t1.join();
    t2.join();
}

//线程安全队列
template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue()
	{}
	threadsafe_queue(threadsafe_queue const& other)
	{
		std::lock_guard<std::mutex> lk(other.mut);
		data_queue = other.data_queue;
	}
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(new_value);
		data_cond.notify_one();
	}
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = data_queue.front();
		data_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = data_queue.front();
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};


void test_safe_que() {
	threadsafe_queue<int>  safe_que;
	std::mutex  mtx_print;
	std::thread producer(
		[&]() {
			for (int i = 0; ; i++) {
				safe_que.push(i);
				{
					std::lock_guard<std::mutex> printlk(mtx_print);
					std::cout << "producer push data is " << i << std::endl;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}
	);

	std::thread consumer1(
		[&]() {
			for (;;) {
				auto data = safe_que.wait_and_pop();
				{
					std::lock_guard<std::mutex> printlk(mtx_print);
					std::cout << "consumer1 wait and pop data is " << *data << std::endl;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	);

	std::thread consumer2(
		[&]() {
			for (;;) {
				auto data = safe_que.try_pop();
				if (data != nullptr) {
					{
						std::lock_guard<std::mutex> printlk(mtx_print);
						std::cout << "consumer2 try_pop data is " << *data << std::endl;
					}

				}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
	);

	producer.join();
	consumer1.join();
	consumer2.join();
}
int main()
{
    std::cout << "Hello World!\n";
}


// thread_02_mutex.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <mutex>
#include <stack>
std::mutex  mtx1;
int shared_data = 100;
void use_lock() {
    while (true) {
        mtx1.lock();
        shared_data++;
        std::cout << "current thread is " << std::this_thread::get_id() << std::endl;
        std::cout << "sharad data is " << shared_data << std::endl;
        mtx1.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}


void test_lock() {
    std::thread t1(use_lock);
    std::thread t2([]() {
        while (true) {
            mtx1.lock();
            shared_data--;
            std::cout << "current thread is " << std::this_thread::get_id() << std::endl;
            std::cout << "sharad data is " << shared_data << std::endl;
            mtx1.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        });
    t1.join();
    t2.join();
}

void test_lockguard() {
    std::thread t3([]() {
        while (true) {
            {
                std::lock_guard<std::mutex>lk_mt(mtx1);
                shared_data--;
                std::cout << "current thread is " << std::this_thread::get_id() << std::endl;
                std::cout << "sharad data is " << shared_data << std::endl;
            }
           
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        });
    t3.join();
}

struct empty_stack:std::exception
{
    const char* what() const throw();
};

template<typename T>
class threadsafe_stack1
{
public:
    threadsafe_stack1() {};
    threadsafe_stack1(const threadsafe_stack1& other) {
        std::lock_guard<std::mutex> lock(other.mtx1);
        data = other.data;
    }
    threadsafe_stack1& operator = (const threadsafe_stack1&) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(mtx1);
        data.push(std::move(new_value));

    }
    ~threadsafe_stack1() {
    };

    T pop_no_full_safe() {
        std::lock_guard<std::mutex>lc_gd(mtx1);
        if (data.empty()) {
            throw empty_stack();
        }
        auto element = data.top();
        data.pop();
        return element;//这里的T的返回可能会导致栈溢出
    }

    std::shared_ptr<T>pop_safe() {
        std::lock_guard<std::mutex>lc_gd(mtx1);
        if (data.empty()) {
            throw nullptr;

        }
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;//make_shared是匿名函数执行后传入shared_ptr可以有效制止反复拷贝
    }

    void pop_safe2(T& value) {
        std::lock_guard<std::mutex>lc_gd(mtx1);
        if (data.empty()) {
            throw nullptr;

        }
        value = data.top();
        data.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx1);
        return data.empty();
    }
private:
    std::stack<T> data;
    mutable std::mutex mtx1;

};

std::mutex  t_lock1;
std::mutex  t_lock2;
int m_1 = 0;
int m_2 = 1;
void dead_lock1() {
    while (true) {
        std::cout << "dead_lock1 begin " << std::endl;
        //先1后2
        t_lock1.lock();
        m_1 = 1024;
        t_lock2.lock();
        m_2 = 2048;
        //应该先解锁临近的锁
        t_lock2.unlock();
        t_lock1.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::cout << "dead_lock2 end " << std::endl;
    }
}
void dead_lock2() {
    while (true) {
        std::cout << "dead_lock2 begin " << std::endl;
        //先2后1，导致死锁，无法交出锁的管理权
        t_lock2.lock();
        m_2 = 2048;
        t_lock1.lock();
        m_1 = 1024;
        t_lock1.unlock();
        t_lock2.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::cout << "dead_lock2 end " << std::endl;
    }
}
void test_dead_lock() {
    //必然造成死锁
    std::thread t1(dead_lock1);
    std::thread t2(dead_lock2);
    t1.join();
    t2.join();
}

//加锁和解锁作为原子操作解耦合，各自只管理自己的功能，将加锁和解锁分装在同一个函数可以避免死锁。
void atomic_lock1() {
    std::cout << "lock1 begin lock" << std::endl;
    t_lock1.lock();
    m_1 = 1024;
    t_lock1.unlock();
    std::cout << "lock1 end lock" << std::endl;
}
void atomic_lock2() {
    std::cout << "lock2 begin lock" << std::endl;
    t_lock2.lock();
    m_2 = 2048;
    t_lock2.unlock();
    std::cout << "lock2 end lock" << std::endl;
}
void safe_lock1() {
    while (true) {
        atomic_lock1();
        atomic_lock2();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
void safe_lock2() {
    while (true) {
        atomic_lock2();
        atomic_lock1();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
void test_safe_lock() {
    std::thread t1(safe_lock1);
    std::thread t2(safe_lock2);
    t1.join();
    t2.join();
}


class some_big_func {
public:
    some_big_func(int data) :_data(data) {

    }
    some_big_func(const some_big_func& bf):_data(bf._data) {

    }
    some_big_func(const some_big_func&& bf) :_data(std::move(bf._data)) {

    }

    friend std::ostream& operator <<(std::ostream& os, const some_big_func& bf) {
        os << bf._data;
        return os;
    }

    some_big_func& operator = (const some_big_func& b2) {
        if (this == &b2) {
            //this 是一个指向当前对象的指针（some_big_func*），
            // 而 *this 则是对当前对象本身的引用（some_big_func&),
            // 返回*this则是返回一个对象引用，方便再次使用
            return *this;
        }
        _data = b2._data;
        return *this;
    }
    some_big_func& operator = (const some_big_func&& b2) {
        _data = std::move(b2._data);
        return *this;
    }

    friend void swap(some_big_func& b1, some_big_func& b2) {
        some_big_func temp = std::move(b1);
        b1 = std::move(b2);
        b2 = temp;
    }
private:
    int _data;
};
//假设这是一个结构包含了锁与复杂的成员对象
class some_func_mrg
{
public:
    some_func_mrg(int data =0):_obj(data) {

    }
    ~some_func_mrg() {};

    void printinfo() {
        std::cout << "current obj data is" << _obj << std::endl;
    }

    friend void danger_swap(some_func_mrg& objm1, some_func_mrg& objm2);
    
    friend void safe_swap(some_func_mrg& objm1, some_func_mrg& objm2);
   
    
    friend void safe_swap_scope(some_func_mrg& objm1, some_func_mrg& objm2);


   
private:
    std::mutex _mutex;
    some_big_func _obj;
};

void safe_swap_scope(some_func_mrg& objm1, some_func_mrg& objm2) {
    std::cout << "thread [ " << std::this_thread::get_id() << " ] begin" << std::endl;
    if (&objm1 == &objm2) {
        return;
    }
    //c++17标准提供的简单方法
    std::scoped_lock guard(objm1._mutex, objm2._mutex);
    swap(objm1._obj, objm2._obj);
    std::cout << "thread [ " << std::this_thread::get_id() << " ] end" << std::endl;

}

void safe_swap(some_func_mrg& objm1, some_func_mrg& objm2) {
    std::cout << "thread [ " << std::this_thread::get_id() << " ] begin" << std::endl;
    if (&objm1 == &objm2) {
        return;
    }
    //如果不可避免需要在一个函数内使用两把锁安全的一种方式就是同时持有防止死锁
    std::lock(objm1._mutex, objm2._mutex);
    //领养锁管理它自动释放
    std::lock_guard <std::mutex> gurad1(objm1._mutex, std::adopt_lock);
    //此处为了故意制造死锁，我们让线程小睡一会
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard <std::mutex> gurad2(objm2._mutex, std::adopt_lock);
    swap(objm1._obj, objm2._obj);
    std::cout << "thread [ " << std::this_thread::get_id() << " ] end" << std::endl;
}

void danger_swap(some_func_mrg& objm1, some_func_mrg& objm2) {
    std::cout << "thread [ " << std::this_thread::get_id() << " ] begin" << std::endl;
    if (&objm1 == &objm2) {
        return;
    }
    std::lock_guard <std::mutex> gurad1(objm1._mutex);
    //此处为了故意制造死锁，我们让线程小睡一会
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::lock_guard<std::mutex> guard2(objm2._mutex);
    swap(objm1._obj, objm2._obj);
    std::cout << "thread [ " << std::this_thread::get_id() << " ] end" << std::endl;
}

void test_danger_swap() {
    some_func_mrg objm1(5);
    some_func_mrg objm2(100);

    std::thread t1(danger_swap, std::ref(objm1), std::ref(objm2));
    std::thread t2(danger_swap, std::ref(objm2), std::ref(objm1));
    t1.join();
    t2.join();

    objm1.printinfo();
    objm2.printinfo();
}

void test_safe_swap() {
    some_func_mrg objm1(5);
    some_func_mrg objm2(100);
    std::thread t1(safe_swap, std::ref(objm1), std::ref(objm2));
    std::thread t2(safe_swap, std::ref(objm2), std::ref(objm1));
    t1.join();
    t2.join();
    objm1.printinfo();
    objm2.printinfo();
}

void test_safe_swap_scope() {
    some_func_mrg objm1(5);
    some_func_mrg objm2(100);
    std::thread t1(safe_swap_scope, std::ref(objm1), std::ref(objm2));
    std::thread t2(safe_swap_scope, std::ref(objm2), std::ref(objm1));
    t1.join();
    t2.join();
    objm1.printinfo();
    objm2.printinfo();
}


//层级锁
class hierarchical_mutex {
public:
    hierarchical_mutex(unsigned long value):_hierachy_value(value),
    _previous_value(0){};

    hierarchical_mutex(const hierarchical_mutex&) = delete;
    hierarchical_mutex& operator =(const hierarchical_mutex&) = delete;

    void lock() {
        check_for_hierarchy_violation();
        _internal_mutex.lock();
        update_hierarchy_violation();
   
    }

    void unlock() {
        if (_this_thread_hierarchy_value != _hierachy_value) {
            throw std::logic_error("mutex hierarchy error");
        }

        _this_thread_hierarchy_value = _previous_value;
        _internal_mutex.unlock();
    }

    bool trylock() {
        check_for_hierarchy_violation();
        if (!_internal_mutex.try_lock()) {
            return false;
        }


        update_hierarchy_violation();
        return true;
    }
private:
    std::mutex _internal_mutex;
    //当前层级值
    unsigned long const _hierachy_value;
    unsigned long _previous_value;
    //本线程记录的层级值
    static thread_local  unsigned long  _this_thread_hierarchy_value;

    void check_for_hierarchy_violation() {
        if (_this_thread_hierarchy_value <= _hierachy_value) {
            throw std::logic_error("mutex hierarchy error");
        }
    }
    void update_hierarchy_violation() {
        _previous_value = _this_thread_hierarchy_value;
        _this_thread_hierarchy_value = _hierachy_value;
    }
};

thread_local unsigned long hierarchical_mutex::_this_thread_hierarchy_value(ULONG_MAX);

void test_hierarchy_lock() {
    hierarchical_mutex  hmtx1(1000);
    hierarchical_mutex  hmtx2(500);
    std::thread t1([&hmtx1, &hmtx2]() {
        hmtx1.lock();
        hmtx2.lock();
        hmtx2.unlock();
        hmtx1.unlock();
        });
    std::thread t2([&hmtx1, &hmtx2]() {
        hmtx2.lock();
        hmtx1.lock();
        hmtx1.unlock();
        hmtx2.unlock();
        });
    t1.join();
    t2.join();
}
int main()
{
    test_hierarchy_lock();
}


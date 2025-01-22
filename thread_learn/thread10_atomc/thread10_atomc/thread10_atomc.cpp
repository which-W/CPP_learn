// thread10_atomc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>
#include <vector>
#include <mutex>
//自旋锁
class SpinLock {
public:
    void lock() {
        //1 处
        while (flag.test_and_set(std::memory_order_acquire)); // 自旋等待，直到成功获取到锁
    }
    void unlock() {
        //2 处
        flag.clear(std::memory_order_release); // 释放锁
    }
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};


void TestSpinLock() {
    SpinLock spinlock;
    std::thread t1([&spinlock]() {
        spinlock.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "*";
        }
        std::cout << std::endl;
        spinlock.unlock();
        });
    std::thread t2([&spinlock]() {
        spinlock.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "?";
        }
        std::cout << std::endl;
        spinlock.unlock();
        });
    t1.join();
    t2.join();
}


std::atomic<bool> x, y;
std::atomic<int> z;
//memory_order_relaxed是宽松的内存序列，它只保证操作的原子性，
// 并不能保证多个变量之间的顺序性，也不能保证同一个变量在不同线程之间的可见顺序。
void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);  // 1
    y.store(true, std::memory_order_relaxed);  // 2
}
void read_y_then_x() {
    while (!y.load(std::memory_order_relaxed)) { // 3
        std::cout << "y load false" << std::endl;
    }
    if (x.load(std::memory_order_relaxed)) { //4
        ++z;
    }
}
//有可能因为x,y的存储顺序的未按预期执行而导致断言出错
void TestOrderRelaxed() {
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load() != 0); // 5
}


//memory_order_seq_cst则能保证所有线程看到的执行顺序是一致的。
//实现 sequencial consistent 模型有一定的开销. 现代 CPU 通常有多核, 每个核心还有自己的缓存. 
// 为了做到全局顺序一致, 每次写入操作都必须同步给其他核心. 
// 为了减少性能开销, 如果不需要全局顺序一致, 我们应该考虑使用更加宽松的顺序模型.
void write_x_then_y() {
    x.store(true, std::memory_order_seq_cst);  // 1
    y.store(true, std::memory_order_seq_cst);  // 2
}
void read_y_then_x() {
    while (!y.load(std::memory_order_seq_cst)) { // 3
        std::cout << "y load false" << std::endl;
    }
    if (x.load(std::memory_order_seq_cst)) { //4
        ++z;
    }
}
void TestOrderSeqCst() {
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load() != 0); // 5
}

//acquire-release的同步内存序
//Acquire-release 可以实现 synchronizes-with 的关系. 
// 如果一个 acquire 操作在同一个原子变量上读取到了一个 release 操作写入的值, 则这个 release 操作 “synchronizes-with” 这个 acquire 操作.
//任何指令都不能重排到 acquire 操作的前面, 且不能重排到 release 操作的后面; 否则会违反 acquire - release 的语义.
// 因此很多需要实现 synchronizes - with 关系的场景都会使用 acquire - release.

//t1执行到2将ry 设置为true, 因为使用了Acquire-release 顺序， 所以 t2 执行到3时读取ry为true， 因此2和3 可以构成同步关系。
//又因为单线程t1内 1 sequence before 2, 所以1 happens - before 3.
//因为单线程t2内 3 sequence before 4. 所以 1 happens-before 4.
//可以断定4 不会触发断言。
//并且当编译器读到release时就一定会先执行1，再执行2会确保release前的操作先执行
void TestReleaseAcquire() {
    std::atomic<bool> rx, ry;
    std::thread t1([&]() {
        rx.store(true, std::memory_order_relaxed); // 1
        ry.store(true, std::memory_order_release); // 2
        });
    std::thread t2([&]() {
        while (!ry.load(std::memory_order_acquire)); //3
        assert(rx.load(std::memory_order_relaxed)); //4
        });
    t1.join();
    t2.join();
}

//我们可以看到t3在yd为true的时候才会退出，那么导致yd为true的有两种情况，一种是1，另一种是2， 所以5处可能触发断言。
//如果2和4相对应则不会触发，但如果3和4相对应则1还没有执行就执行5了会触发断言
void ReleasAcquireDanger2() {
    std::atomic<int> xd{ 0 }, yd{ 0 };
    std::atomic<int> zd;
    std::thread t1([&]() {
        xd.store(1, std::memory_order_release);  // (1)
        yd.store(1, std::memory_order_release); //  (2)
        });
    std::thread t2([&]() {
        yd.store(2, std::memory_order_release);  // (3)
        });
    std::thread t3([&]() {
        while (!yd.load(std::memory_order_acquire)); //（4）
        assert(xd.load(std::memory_order_acquire) == 1); // (5)
        });
    t1.join();
    t2.join();
    t3.join();
}


//t2执行到(4)处时，需要等到ptr非空才能退出循环，这就依赖t1执行完(3)操作。
//
//因此(3) “dependency - ordered before”(4), 根据前文我们介绍了dependency等同于synchronizes ，所以(3) “inter - thread happens - before”.（4）
//
//因为(1) “sequenced before”(3), 所以(1) “happens - before “(4)
//
//因为(4) “sequenced before”(5), 所以(1) “happens - before “(5)
//
//所以(5)处断言也不会触发。而（6）处的断言因为data和p没有依赖关系所以可能会触发
//
//因为(2) 和(3)不构成先行关系，所以(6)处断言可能触发。
void ReleaseSequence() {
    std::vector<int> data;
    std::atomic<int> flag{ 0 };
    std::thread t1([&]() {
        data.push_back(42);  //(1)
        flag.store(1, std::memory_order_release); //(2)
        });
    std::thread t2([&]() {
        int expected = 1;
        while (!flag.compare_exchange_strong(expected, 2, std::memory_order_relaxed)) // (3)
            expected = 1;
        });
    std::thread t3([&]() {
        while (flag.load(std::memory_order_acquire) < 2); // (4)
        assert(data.at(0) == 42); // (5)
        });
    t1.join();
    t2.join();
    t3.join();
}

//new本身的生成是有顺序的
// 可能为
//1 为对象allocate一块内存空间
//2 调用construct构造对象
//3 将构造到的对象地址返回
// 也可能为
//1 为对象allocate一块内存空间
//2 先将开辟的空间地址返回
//3 调用construct构造对象

//为了防止在单例模式中为构造对象就被直接返回而导致对象的成员函数无法使用的情况所以需要使用内存序来优化单例

//利用智能指针解决释放问题
class SingleMemoryModel
{
private:
    SingleMemoryModel()
    {
    }
    SingleMemoryModel(const SingleMemoryModel&) = delete;
    SingleMemoryModel& operator=(const SingleMemoryModel&) = delete;
public:
    ~SingleMemoryModel()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    static std::shared_ptr<SingleMemoryModel> GetInst()
    {
        // 1 处
        if (_b_init.load(std::memory_order_acquire))
        {
            return single;
        }
        // 2 处
        s_mutex.lock();
        // 3 处
        if (_b_init.load(std::memory_order_relaxed))
        {
            s_mutex.unlock();
            return single;
        }
        // 4处 这里会确保在对象完全构造好之前不会先释放锁
        single = std::shared_ptr<SingleMemoryModel>(new SingleMemoryModel);
        _b_init.store(true, std::memory_order_release);
        s_mutex.unlock();
        return single;
    }
private:
    static std::shared_ptr<SingleMemoryModel> single;
    static std::mutex s_mutex;
    static std::atomic<bool> _b_init;
};
std::shared_ptr<SingleMemoryModel> SingleMemoryModel::single = nullptr;
std::mutex SingleMemoryModel::s_mutex;
std::atomic<bool> SingleMemoryModel::_b_init = false;
int main()
{
  
    TestSpinLock();
}


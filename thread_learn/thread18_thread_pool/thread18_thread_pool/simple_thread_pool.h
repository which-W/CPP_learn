#pragma once
#include <atomic>
#include "ThreadSafeQue.h"
#include "join_thread.h"
#include <functional>
class simple_thread_pool
{
    std::atomic_bool done;
    //⇽-- - 1
    threadsafe_queue<std::function<void()> > work_queue;
    //⇽-- - 2
    std::vector<std::thread> threads;
    //⇽-- - 3
    join_threads joiner;
    void worker_thread()
    {
        //⇽-- - 4
        while (!done)
        {
            std::function<void()> task;
            //⇽-- - 5
            if (work_queue.try_pop(task))
            {
                //⇽-- - 6
                task();
            }
            else
            {
                //⇽-- - 7
                std::this_thread::yield();
            }
        }
    }

    simple_thread_pool() :
        done(false), joiner(threads)
    {
        //⇽--- 8
        unsigned const thread_count = std::thread::hardware_concurrency();
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                //⇽-- - 9
                threads.push_back(std::thread(&simple_thread_pool::worker_thread, this));
            }
        }
        catch (...)
        {
            //⇽-- - 10
            done = true;
            throw;
        }
    }
public:
    static simple_thread_pool& instance() {
        static  simple_thread_pool pool;
        return pool;
    }
    ~simple_thread_pool()
    {
        //⇽-- - 11
        done = true;
        for (unsigned i = 0; i < threads.size(); ++i)
        {
            //⇽-- - 9
            threads[i].join();
        }
    }
    template<typename FunctionType>
    void submit(FunctionType f)
    {
        //⇽-- - 12
        work_queue.push(std::function<void()>(f));
    }
};
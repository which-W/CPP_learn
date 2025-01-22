#pragma once
#include "ThreadSafeQue.h"
#include <future>
#include "join_thread.h"
#include "FutureThreadPool.h"

//自动生产与消费的线程池涉及
class notify_thread_pool
{
private:
	void worker_thread()
	{
		while (!done)
		{

			auto task_ptr = work_queue.wait_and_pop();
			if (task_ptr == nullptr) {
				continue;
			}

			(*task_ptr)();
		}
	}
public:

	static notify_thread_pool& instance() {
		static  notify_thread_pool pool;
		return pool;
	}
	~notify_thread_pool()
	{
		//⇽-- - 11
		done = true;
		work_queue.Exit();
		for (unsigned i = 0; i < threads.size(); ++i)
		{
			//⇽-- - 9
			threads[i].join();
		}
	}

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type>
		submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type result_type;
		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
	}

private:
	notify_thread_pool() :
		done(false), joiner(threads)
	{
		//⇽--- 8
		unsigned const thread_count = std::thread::hardware_concurrency();
		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{
				//⇽-- - 9
				threads.push_back(std::thread(&notify_thread_pool::worker_thread, this));
			}
		}
		catch (...)
		{
			//⇽-- - 10
			done = true;
			work_queue.Exit();
			throw;
		}
	}

	std::atomic_bool done;
	//⇽-- - 1
	threadsafe_queue<function_wrapper> work_queue;
	//⇽-- - 2
	std::vector<std::thread> threads;
	//⇽-- - 3
	join_threads joiner;
};

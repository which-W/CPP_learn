#pragma once
#include "ThreadSafeQue.h"
#include <future>
#include "ThreadSafeQue.h"
#include "join_thread.h"

//可以自主选择何时获取结果的线程池
class function_wrapper
{
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {}
	};
	std::unique_ptr<impl_base> impl;
	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(std::move(f_)) {}
		void call() { f(); }
	};
public:
	template<typename F>
	function_wrapper(F&& f) :
		impl(new impl_type<F>(std::move(f)))
	{}
	void operator()() { impl->call(); }
	function_wrapper() = default;
	function_wrapper(function_wrapper&& other) :
		impl(std::move(other.impl))
	{}
	function_wrapper& operator=(function_wrapper&& other)
	{
		impl = std::move(other.impl);
		return *this;
	}
	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete;
	function_wrapper& operator=(const function_wrapper&) = delete;
};
class future_thread_pool
{
private:
	void worker_thread()
	{
		while (!done)
		{
			function_wrapper task;

			if (work_queue.try_pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}
public:

	static future_thread_pool& instance() {
		static  future_thread_pool pool;
		return pool;
	}
	~future_thread_pool()
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
	future_thread_pool() :
		done(false), joiner(threads)
	{
		//⇽--- 8
		unsigned const thread_count = std::thread::hardware_concurrency();
		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{
				//⇽-- - 9
				threads.push_back(std::thread(&future_thread_pool::worker_thread, this));
			}
		}
		catch (...)
		{
			//⇽-- - 10
			done = true;
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
#pragma once
#include "ThreadSafeQue.h"
#include <future>
#include "ThreadSafeQue.h"
#include "join_thread.h"
#include "FutureThreadPool.h"

//窃取线程池最大效用提高线程利用率（可以进行修改来进一步优化）
class steal_thread_pool
{
private:

	void worker_thread(int index)
	{
		while (!done)
		{
			function_wrapper wrapper;
			bool pop_res = thread_work_ques[index].try_pop(wrapper);
			if (pop_res) {
				wrapper();
				continue;
			}

			bool steal_res = false;
			for (int i = 0; i < thread_work_ques.size(); i++) {
				if (i == index) {
					continue;
				}

				steal_res = thread_work_ques[i].try_steal(wrapper);
				if (steal_res) {
					wrapper();
					break;
				}

			}

			if (steal_res) {
				continue;
			}

			std::this_thread::yield();
		}
	}
public:

	static steal_thread_pool& instance() {
		static  steal_thread_pool pool;
		return pool;
	}
	~steal_thread_pool()
	{
		//⇽-- - 11
		done = true;
		for (unsigned i = 0; i < thread_work_ques.size(); i++) {
			thread_work_ques[i].Exit();
		}

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
		int index = (atm_index.load() + 1) % thread_work_ques.size();
		atm_index.store(index);
		typedef typename std::result_of<FunctionType()>::type result_type;
		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		thread_work_ques[index].push(std::move(task));
		return res;
	}

private:
	steal_thread_pool() :
		done(false), joiner(threads), atm_index(0)
	{
		//⇽--- 8
		unsigned const thread_count = std::thread::hardware_concurrency();
		try
		{
			thread_work_ques = std::vector < threadsafe_queue<function_wrapper>>(thread_count);

			for (unsigned i = 0; i < thread_count; ++i)
			{
				//⇽-- - 9
				threads.push_back(std::thread(&steal_thread_pool::worker_thread, this, i));
			}
		}
		catch (...)
		{
			//⇽-- - 10
			done = true;
			for (int i = 0; i < thread_work_ques.size(); i++) {
				thread_work_ques[i].Exit();
			}
			throw;
		}
	}

	std::atomic_bool done;
	//全局队列
	std::vector<threadsafe_queue<function_wrapper>> thread_work_ques;

	//⇽-- - 2
	std::vector<std::thread> threads;
	//⇽-- - 3
	join_threads joiner;
	std::atomic<int>  atm_index;
};
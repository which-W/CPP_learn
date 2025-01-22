#pragma once
#include <future>
#include <thread>
#include "ThreadPool.h"
#include <vector>
#include "simple_thread_pool.h"
#include "FutureThreadPool.h"
#include "NotifyThreadPool.h"

//使用最完善的线程池 实现并行for-each
template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f)
{
	unsigned long const length = std::distance(first, last);
	if (!length)
		return;
	unsigned long const min_per_thread = 25;
	unsigned long const num_threads =
		(length + min_per_thread - 1) / min_per_thread;

	unsigned long const block_size = length / num_threads;
	std::vector<std::future<void>> futures(num_threads - 1);   //⇽-- - 1
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		futures[i] = ThreadPool::instance().commit([=]()
			{
				std::for_each(block_start, block_end, f);
			});

		block_start = block_end;
	}
	std::for_each(block_start, last, f);
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		futures[i].get();   // ⇽-- - 4
	}
}

template<typename Iterator, typename Func>
void recursion_for_each(Iterator first, Iterator last, Func f)
{
	std::mutex mtx;
	std::vector<std::future<void>> futures;
	unsigned long const length = std::distance(first, last);
	if (!length)
		return;
	unsigned long const min_per_thread = 25;
	if (length < (2 * min_per_thread))
	{
		std::for_each(first, last, f);    //⇽-- - 1
	}
	else
	{
		Iterator const mid_point = first + length / 2;
		{
			std::lock_guard<std::mutex> lock(mtx);
			futures.push_back(
				ThreadPool::instance().commit([=]()
					{
						std::for_each(first, mid_point, f);
					}));
		}

		recursion_for_each(mid_point, last, f);

		for (auto& future : futures) {
			future.get();
		}
	}
}

//简单的轮训线程池实现并行foreach
template<typename Iterator, typename Func>
void simple_for_each(Iterator first, Iterator last, Func f) {
	unsigned long const length = std::distance(first, last);
	if (!length)
		return;
	unsigned long const min_per_thread = 25;
	unsigned long const num_threads =
		(length + min_per_thread - 1) / min_per_thread;

	unsigned long const block_size = length / num_threads;
	std::vector<std::future<void>> futures(num_threads - 1);   //⇽-- - 1
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		simple_thread_pool::instance().submit([=]()
			{
				std::for_each(block_start, block_end, f);
			});

		block_start = block_end;
	}
	std::for_each(block_start, last, f);
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

//简单轮训并且配合future实现的线程池 实现并行for_each
//简单的轮训线程池实现并行foreach
template<typename Iterator, typename Func>
void future_for_each(Iterator first, Iterator last, Func f) {
	unsigned long const length = std::distance(first, last);
	if (!length)
		return;
	unsigned long const min_per_thread = 25;
	unsigned long const num_threads =
		(length + min_per_thread - 1) / min_per_thread;

	unsigned long const block_size = length / num_threads;
	std::vector<std::future<void>> futures(num_threads - 1);   //⇽-- - 1
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		futures[i] = future_thread_pool::instance().submit([=]()
			{
				std::for_each(block_start, block_end, f);
			});

		block_start = block_end;
	}
	std::for_each(block_start, last, f);
	for (auto& future : futures) {
		future.get();
	}
}

//基于作者思路优化的条件变量等待的线程池
template<typename Iterator, typename Func>
void notify_for_each(Iterator first, Iterator last, Func f) {
	unsigned long const length = std::distance(first, last);
	if (!length)
		return;
	unsigned long const min_per_thread = 25;
	unsigned long const num_threads =
		(length + min_per_thread - 1) / min_per_thread;

	unsigned long const block_size = length / num_threads;
	std::vector<std::future<void>> futures(num_threads - 1);   //⇽-- - 1
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		futures[i] = notify_thread_pool::instance().submit([=]()
			{
				std::for_each(block_start, block_end, f);
			});

		block_start = block_end;
	}
	std::for_each(block_start, last, f);
	for (auto& future : futures) {
		future.get();
	}
}
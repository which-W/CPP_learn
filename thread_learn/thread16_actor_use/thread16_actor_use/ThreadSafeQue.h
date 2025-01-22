#pragma once
#include <mutex>
#include <queue>
#include <atomic>
template<typename T>
class ThreadSafeQue
{
private:
	struct node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};

	std::mutex head_mutex;
	std::unique_ptr<node> head;
	std::mutex tail_mutex;
	node* tail;
	std::condition_variable data_cond;

	std::atomic<bool> _bstop;

	node* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}
	std::unique_ptr<node> pop_head()
	{
		std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

	std::unique_lock<std::mutex> wait_for_data()
	{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		data_cond.wait(head_lock, [&] {return (_bstop.load() == true) || (head.get() != get_tail()); });
		return std::move(head_lock);
	}


	std::unique_ptr<node> wait_pop_head()
	{
		std::unique_lock<std::mutex> head_lock(wait_for_data());

		if (_bstop.load()) {
			return nullptr;
		}

		return pop_head();
	}

	std::unique_ptr<node> wait_pop_head(T& value)
	{
		std::unique_lock<std::mutex> head_lock(wait_for_data());

		if (_bstop.load()) {
			return nullptr;
		}

		value = std::move(*head->data);
		return pop_head();
	}


	std::unique_ptr<node> try_pop_head()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<node>();
		}
		return pop_head();
	}
	std::unique_ptr<node> try_pop_head(T& value)
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<node>();
		}
		value = std::move(*head->data);
		return pop_head();
	}
public:

	ThreadSafeQue() :  // ⇽-- - 1
		head(new node), tail(head.get()), _bstop(false)
	{}

	ThreadSafeQue(const ThreadSafeQue& other) = delete;
	ThreadSafeQue& operator=(const ThreadSafeQue& other) = delete;

	void NotifyStop() {
		_bstop.store(true);
		data_cond.notify_one();
	}

	std::shared_ptr<T> WaitAndPop() //  <------3
	{
		std::unique_ptr<node> const old_head = wait_pop_head();
		if (old_head == nullptr) {
			return nullptr;
		}
		return old_head->data;
	}

	void WaitAndPop(T& value)  //  <------4
	{
		std::unique_ptr<node> const old_head = wait_pop_head(value);
	}


	std::shared_ptr<T> Try()
	{
		std::unique_ptr<node> old_head = try_pop_head();
		return old_head ? old_head->data : std::shared_ptr<T>();
	}
	bool try_pop(T& value)
	{
		std::unique_ptr<node> const old_head = try_pop_head(value);
		return old_head;
	}
	bool empty()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		return (head.get() == get_tail());
	}



	void push(T new_value) //<------2
	{
		std::shared_ptr<T> new_data(
			std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<node> p(new node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data = new_data;
			node* const new_tail = p.get();
			tail->next = std::move(p);
			tail = new_tail;
		}

		data_cond.notify_one();
	}
};

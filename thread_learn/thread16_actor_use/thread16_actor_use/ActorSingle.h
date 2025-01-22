#pragma once
#include <thread>
#include "ThreadSafeQue.h"
#include <atomic>
#include <iostream>
template<typename ClassType, typename QueType>
class ActorSingle {
public:
	static ClassType& Inst() {
		static ClassType as;
		return as;
	}

	~ActorSingle() {

	}

	void PostMsg(const QueType& data) {
		_que.push(data);
	}

protected:

	ActorSingle() :_bstop(false) {

	}

	ActorSingle(const ActorSingle&) = delete;
	ActorSingle(ActorSingle&&) = delete;
	ActorSingle& operator = (const ActorSingle&) = delete;

	std::atomic<bool> _bstop;
	ThreadSafeQue<QueType>  _que;
	std::thread _thread;
};

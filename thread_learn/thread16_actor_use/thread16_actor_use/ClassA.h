#pragma once
#include "ActorSingle.h"
#include "ClassB.h"

struct MsgClassA {
	std::string name;
	friend std::ostream& operator << (std::ostream& os, const MsgClassA& ca) {
		os << ca.name;
		return os;
	}
};


class ClassA : public ActorSingle<ClassA, MsgClassA> {
	friend class ActorSingle<ClassA, MsgClassA>;
public:
	~ClassA() {
		_bstop = true;
		_que.NotifyStop();
		_thread.join();
		std::cout << "ClassA destruct " << std::endl;
	}

	void DealMsg(std::shared_ptr<MsgClassA> data) {
		std::cout << "class A deal msg is " << *data << std::endl;

		MsgClassB msga;
		msga.name = "llfc";
		ClassB::Inst().PostMsg(msga);
	}
private:
	ClassA() {
		_thread = std::thread([this]() {
			for (; (_bstop.load() == false);) {
				std::shared_ptr<MsgClassA> data = _que.WaitAndPop();
				if (data == nullptr) {
					continue;
				}

				DealMsg(data);
			}

			std::cout << "ClassA thread exit " << std::endl;
			});
	}
};
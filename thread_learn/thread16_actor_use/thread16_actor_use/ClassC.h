#pragma once
#include "ActorSingle.h"

struct MsgClassC {
	std::string name;
	friend std::ostream& operator << (std::ostream& os, const MsgClassC& ca) {
		os << ca.name;
		return os;
	}
};


class ClassC : public ActorSingle<ClassC, MsgClassC> {
	friend class ActorSingle<ClassC, MsgClassC>;
public:
	~ClassC() {
		_bstop = true;
		_que.NotifyStop();
		_thread.join();
		std::cout << "ClassC destruct " << std::endl;
	}

	void DealMsg(std::shared_ptr<MsgClassC> data) {
		std::cout << "class C deal msg is " << *data << std::endl;
	}
private:
	ClassC() {
		_thread = std::thread([this]() {
			for (; (_bstop.load() == false);) {
				std::shared_ptr<MsgClassC> data = _que.WaitAndPop();
				if (data == nullptr) {
					continue;
				}

				DealMsg(data);
			}

			std::cout << "ClassC thread exit " << std::endl;
			});
	}
};
#pragma once
#include "global.h"
#include "TaskManager.h"
// CRTP »ùÀàÄ£°å
template<typename Derived>
class Command
{
public:
	void execute(const std::string& args)
	{
		(static_cast<Derived*>(this))->executeImpl(args);
	}
private:

};

class AddCommand:public Command<AddCommand>
{
public:
	AddCommand(TaskManager& manager) :taskManager(manager) {};
	void executeImpl(const std::string& args);

private:
	TaskManager& taskManager;
};

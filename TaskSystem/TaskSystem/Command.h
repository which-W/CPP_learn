#pragma once
#include "global.h"
#include "TaskManager.h"
// CRTP 基类模板
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
// 添加任务命令
class AddCommand:public Command<AddCommand>
{
public:
	AddCommand(TaskManager& manager) :taskManager(manager) {};
	void executeImpl(const std::string& args);

private:
	TaskManager& taskManager;
};

// 删除任务命令
class DeleteCommand : public Command<DeleteCommand> {
public:
    DeleteCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args);
private:
    TaskManager& taskManager;
};

// 列出任务命令
class ListCommand : public Command<ListCommand> {
public:
    ListCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args);
private:
    TaskManager& taskManager;
};

// 更新任务命令
class UpdateCommand : public Command<UpdateCommand> {
public:
    UpdateCommand(TaskManager& manager) : taskManager(manager) {};
    void executeImpl(const std::string& args);
private:
    TaskManager& taskManager;
};

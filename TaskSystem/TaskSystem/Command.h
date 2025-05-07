#pragma once
#include "global.h"
#include "TaskManager.h"
// CRTP ����ģ��
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
// �����������
class AddCommand:public Command<AddCommand>
{
public:
	AddCommand(TaskManager& manager) :taskManager(manager) {};
	void executeImpl(const std::string& args);

private:
	TaskManager& taskManager;
};

// ɾ����������
class DeleteCommand : public Command<DeleteCommand> {
public:
    DeleteCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args);
private:
    TaskManager& taskManager;
};

// �г���������
class ListCommand : public Command<ListCommand> {
public:
    ListCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args);
private:
    TaskManager& taskManager;
};

// ������������
class UpdateCommand : public Command<UpdateCommand> {
public:
    UpdateCommand(TaskManager& manager) : taskManager(manager) {};
    void executeImpl(const std::string& args);
private:
    TaskManager& taskManager;
};

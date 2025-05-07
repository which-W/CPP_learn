// TaskSystem.cpp: 定义应用程序的入口点。
//
#include "global.h"
#include "TaskManager.h"
#include "Command.h"
#include "Logger.h"
#include "CommandWrapper.h"
int main()
{
	TaskManager taskManager;
	// CRTP构建
	std::unordered_map<std::string, CommandWrapper> commandMap;
	commandMap.emplace("add", AddCommand(taskManager));
	commandMap.emplace("delete", DeleteCommand(taskManager));
	commandMap.emplace("list", ListCommand(taskManager));
	commandMap.emplace("update", UpdateCommand(taskManager));
	commandMap.emplace("upgrate_index", Update_IndexCommand(taskManager));
	std::cout << "欢迎使用任务管理系统！" << std::endl;
	std::cout << "可用命令: add, delete, list, update, exit, upgrate_index" << std::endl;
	std::string input;
	while (true) {
		std::cout << "\n>";
		std::getline(std::cin, input);
		if (input == "") {
			continue;
		}
		if (input == "exit") {
			std::cout << "退出任务管理系统。" << std::endl;
			break;
		}
		// 分割命令和参数
		size_t spacePos = input.find(' ');
		std::string cmd = input.substr(0, spacePos);
		std::string args = (spacePos != std::string::npos) ? input.substr(spacePos + 1) : "";
		// 查找命令
		if (cmd == "exit") {
			std::cout << "退出程序。" << std::endl;
			break;
		}
		auto it = commandMap.find(cmd);
		if (it != commandMap.end()) {
			it->second.execute(args);
		}
		else {
			std::cout << "未知命令：" << cmd << std::endl;
		}

	}
	return 0;
}

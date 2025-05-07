#include "Command.h"
#include "TaskManager.h"
void AddCommand::executeImpl(const std::string& args) {
    // 简单的参数解析：描述,优先级,截止日期
    //找到两个逗号的位置
    size_t pos1 = args.find(',');
    size_t pos2 = args.find(',', pos1 + 1);
    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        std::cout << "参数格式错误。请使用: add <描述>,<优先级>,<截止日期>" << std::endl;
        return;
    }
    // 截取描述，优先级和截至时间
    std::string description = args.substr(0, pos1);
    int priority = std::stoi(args.substr(pos1 + 1, pos2 - pos1 - 1));
    std::string dueDate = args.substr(pos2 + 1);
    std::cout << description << std::endl;
    std::cout << priority << std::endl;
    std::cout << dueDate << std::endl;
    taskManager.addTask(description, priority, dueDate);
    std::cout << "任务添加成功。" << std::endl;
}

void DeleteCommand::executeImpl(const std::string& args) {
    try {
        size_t pos;
        int id = std::stoi(args, &pos);
        if (pos != args.length()) {
            std::cout << "参数格式错误。请使用: delete <ID>" << std::endl;
            return;
        }
        taskManager.deleteTask(id);
        std::cout << "任务删除成功。" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "参数格式错误。请使用: delete <ID>" << std::endl;
        return;
    }
    catch (const std::out_of_range& e) {
        std::cout << "ID超出范围。请使用有效的任务ID。" << std::endl;
        return;
    }
    catch (const std::exception& e) {
        std::cout << "删除任务时发生异常: " << e.what() << std::endl;
        return;
    }
    catch (...) {
        std::cout << "发生未知错误。" << std::endl;
        return;
    }
}

void ListCommand::executeImpl(const std::string& args) {
    int sortOption = 0;
    if (!args.empty()) {
        sortOption = std::stoi(args);
    }
    taskManager.listTasks(sortOption);
}

void UpdateCommand::executeImpl(const std::string& args) {
    // 参数格式: ID,描述,优先级,截止日期
    size_t pos1 = args.find(',');
    size_t pos2 = args.find(',', pos1 + 1);
    size_t pos3 = args.find(',', pos2 + 1);
    if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos) {
        std::cout << "参数格式错误。请使用: update <ID>,<描述>,<优先级>,<截止日期>" << std::endl;
        return;
    }
    int id = std::stoi(args.substr(0, pos1));
    std::string description = args.substr(pos1 + 1, pos2 - pos1 - 1);
    int priority = std::stoi(args.substr(pos2 + 1, pos3 - pos2 - 1));
    std::string dueDate = args.substr(pos3 + 1);
    taskManager.updateTask(id, description, priority, dueDate);
    std::cout << "任务更新成功。" << std::endl;
}

void Update_IndexCommand::executeImpl(const std::string& args) {
    taskManager.upgrate_index();
    std::cout << "索引更新成功。" << std::endl;
}
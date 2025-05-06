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
    taskManager.addTask(description, priority, dueDate);
    std::cout << "任务添加成功。" << std::endl;
}
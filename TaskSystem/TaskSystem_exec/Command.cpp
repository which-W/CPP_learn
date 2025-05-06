#include "Command.h"
#include "TaskManager.h"
void AddCommand::executeImpl(const std::string& args) {
    // �򵥵Ĳ�������������,���ȼ�,��ֹ����
    //�ҵ��������ŵ�λ��
    size_t pos1 = args.find(',');
    size_t pos2 = args.find(',', pos1 + 1);
    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        std::cout << "������ʽ������ʹ��: add <����>,<���ȼ�>,<��ֹ����>" << std::endl;
        return;
    }
    // ��ȡ���������ȼ��ͽ���ʱ��
    std::string description = args.substr(0, pos1);
    int priority = std::stoi(args.substr(pos1 + 1, pos2 - pos1 - 1));
    std::string dueDate = args.substr(pos2 + 1);
    taskManager.addTask(description, priority, dueDate);
    std::cout << "������ӳɹ���" << std::endl;
}
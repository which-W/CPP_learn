#include"TaskManager.h"
#include "nlohmann/json.hpp"
// ʹ�� nlohmann �����ռ�򻯴���
using nlohmann::json;

TaskManager::TaskManager():next_Id(1)
{
	loadTasks();

}
void TaskManager::loadTasks()
{
    std::ifstream inFile("tasks.json");
    if (!inFile) {
        Logger::GetInstance().log("�޷��������ļ��������ǵ�һ�����С�");
        return;
    }
    // ��ȡ�����ļ�����
    std::string jsonData((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());

    // �ر��ļ�
    inFile.close();

    // ���� JSON ����
    try {
        json jsonDataParsed = json::parse(jsonData);

        // ���� JSON �����е�ÿ���������
        for (const auto& item : jsonDataParsed) {
            Task task;
            task.id = item["id"].get<int>();
            task.description = item["description"].get<std::string>();
            task.priority = item["priority"].get<int>();
            task.dueDate = item["dueDate"].get<std::string>();

            tasks.push_back(task);
            Logger::GetInstance().log("��������ɹ���");
            if (task.id >= nextId) {
                nextId = task.id + 1;
            }
        }
        catch (json::parse_error& ec) { // �����������
            std::cerr << "JSON parse error: " << ec.what() << std::endl;
			Logger::GetInstance().log("JSON��������: " + std::string(ec.what()));
			return;
        }

    }

}

void TaskManager::addTask(const std::string& description, int priority, const std::string& dueDate)
{
	Task task;
	task.id = next_Id++;
	task.description = description;
	task.priority = priority;
	task.dueDate = dueDate;

	tasks.push_back(task);
	Logger::GetInstance().log("�������ɹ���","task add {}",task.toString());
    saveTasks();//���������ļ�
}

void TaskManager::saveTasks() const
{
	std::ofstream outFile("tasks.json");
	if (!outFile) {
		Logger::GetInstance().log("�޷��������ļ����б��档");
		return;
	}

	// ���� JSON ����
	json jsonData = json::array();

	// ���������б���ÿ������ת��Ϊ JSON ����
	for (const auto& task : tasks) {
		json taskJson;
		taskJson["id"] = task.id;
		taskJson["description"] = task.description;
		taskJson["priority"] = task.priority;
		taskJson["dueDate"] = task.dueDate;

		jsonData.push_back(taskJson);
	}

	// �� JSON ����д���ļ�
	outFile << jsonData.dump(4); // 4 ����������

	// �ر��ļ�
	outFile.close();
	Logger::GetInstance().log("��������ɹ���");
}

void TaskManager::listTasks(int sortOption) const
{
	std::vector<Task> sortedTasks = tasks;

	// ��������ѡ���������
	switch (sortOption) {
	case 1:
		std::sort(sortedTasks.begin(), sortedTasks.end(), compareByPriority);
		break;
	case 2:
		std::sort(sortedTasks.begin(), sortedTasks.end(), compareByDueDate);
		break;
	default:
		break; // Ĭ�ϰ� ID ����
	}

	// ��ӡ�����б�
	for (const auto& task : sortedTasks) {
		std::cout << task.toString() << std::endl;
	}
}

void TaskManager::updateTask(int id, const std::string& description, int priority, const std::string& dueDate) {
	for (auto& task : tasks) {
		if (task.id == id) {
			Logger::GetInstance().log("����ǰ����: " + task.toString());
			task.description = description;
			task.priority = priority;
			task.dueDate = dueDate;
			Logger::GetInstance().log("���º�����: " + task.toString());
			saveTasks();
			return;
		}
	}
	std::cout << "δ�ҵ�IDΪ " << id << " ������" << std::endl;
}

void TaskManager::deleteTask(int id) {
	auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& task) {
		return task.id == id;
		});
	if (it != tasks.end()) {
		tasks.erase(it);
		Logger::GetInstance().log("ɾ������: " + it->toString());
		saveTasks();
	}
	else {
		std::cout << "δ�ҵ�IDΪ " << id << " ������" << std::endl;
	}
}

bool TaskManager::compareByPriority(const Task& a, const Task& b) {
	return a.priority < b.priority;
}

bool TaskManager::compareByDueDate(const Task& a, const Task& b) {
	return a.dueDate < b.dueDate;
}
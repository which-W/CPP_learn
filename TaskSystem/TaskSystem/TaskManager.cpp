#include"TaskManager.h"
#include "nlohmann/json.hpp"
// 使用 nlohmann 命名空间简化代码
using json = nlohmann::json;
TaskManager::TaskManager():next_Id(1)
{
	loadTasks();

}
void TaskManager::loadTasks()
{
    std::ifstream inFile("tasks.json");
    if (!inFile) {
        Logger::GetInstance().log(LogLevel::INFO,"无法打开任务文件，可能是第一次运行。");
        return;
    }
    // 读取整个文件内容
    std::string jsonData((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());

    // 关闭文件
    inFile.close();

    // 解析 JSON 数据
	try {
		json jsonDataParsed = json::parse(jsonData);

		// 遍历 JSON 数组中的每个任务对象
		for (const auto& item : jsonDataParsed) {
			Task task;
			task.id = item["id"].get<int>();
			task.description = item["description"].get<std::string>();
			task.priority = item["priority"].get<int>();
			task.dueDate = item["dueDate"].get<std::string>();

			tasks.push_back(task);
			Logger::GetInstance().log(LogLevel::INFO, "加载任务成功。");
			if (task.id >= next_Id) {
				next_Id = task.id + 1;
			}
		}
	}
	catch (const json::parse_error& e) {
		Logger::GetInstance().log(LogLevel::ERROR, "JSON解析错误: {}", e.what());
	}
	catch (const std::exception& e) {
		Logger::GetInstance().log(LogLevel::ERROR, "加载任务时发生错误: {}", e.what());
	}
	catch (...) {
		Logger::GetInstance().log(LogLevel::ERROR, "加载任务时发生未知错误。");
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
	Logger::GetInstance().log(LogLevel::INFO,"添加任务成功。","task add {}",task.toString());
    saveTasks();//保存任务到文件
}

void TaskManager::saveTasks() const
{
	std::ofstream outFile("tasks.json");
	if (!outFile) {
		Logger::GetInstance().log(LogLevel::INFO,"无法打开任务文件进行保存。");
		return;
	}

	// 创建 JSON 数组
	json jsonData = json::array();

	// 遍历任务列表，将每个任务转换为 JSON 对象
	for (const auto& task : tasks) {
		json taskJson;
		taskJson["id"] = task.id;
		taskJson["description"] = task.description;
		taskJson["priority"] = task.priority;
		taskJson["dueDate"] = task.dueDate;

		jsonData.push_back(taskJson);
	}

	// 将 JSON 数据写入文件
	outFile << jsonData.dump(4); // 4 是缩进级别

	// 关闭文件
	outFile.close();
	Logger::GetInstance().log(LogLevel::INFO,"保存任务成功。");
}

void TaskManager::listTasks(int sortOption) const
{
	std::vector<Task> sortedTasks = tasks;

	// 根据排序选项进行排序
	switch (sortOption) {
	case 1:
		std::sort(sortedTasks.begin(), sortedTasks.end(), compareByPriority);
		break;
	case 2:
		std::sort(sortedTasks.begin(), sortedTasks.end(), compareByDueDate);
		break;
	default:
		break; // 默认按 ID 排序
	}

	// 打印任务列表
	for (const auto& task : sortedTasks) {
		std::cout << task.toString() << std::endl;
	}
}

void TaskManager::updateTask(int id, const std::string& description, int priority, const std::string& dueDate) {
	for (auto& task : tasks) {
		if (task.id == id) {
			Logger::GetInstance().log(LogLevel::INFO,"更新前任务: " + task.toString());
			task.description = description;
			task.priority = priority;
			task.dueDate = dueDate;
			Logger::GetInstance().log(LogLevel::INFO,"更新后任务: " + task.toString());
			saveTasks();
			return;
		}
	}
	std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
}

void TaskManager::deleteTask(int id) {
	auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& task) {
		return task.id == id;
		});
	if (it != tasks.end()) {
		tasks.erase(it);
		Logger::GetInstance().log(LogLevel::INFO,"删除任务: " + it->toString());
		saveTasks();
	}
	else {
		std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
	}
}

bool TaskManager::compareByPriority(const Task& a, const Task& b) {
	return a.priority < b.priority;
}

bool TaskManager::compareByDueDate(const Task& a, const Task& b) {
	return a.dueDate < b.dueDate;
}
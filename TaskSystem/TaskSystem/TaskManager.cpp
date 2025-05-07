#include"TaskManager.h"
#include "../includes/nlohmann/json.hpp"
// 使用 nlohmann 命名空间简化代码
using json = nlohmann::json;
TaskManager::TaskManager() :next_Id(1)
{
	loadTasks();

}

void TaskManager::loadTasks()
{
	std::ifstream inFile("tasks.json");
	if (!inFile.is_open()) {
		Logger::GetInstance().log(LogLevel::INFO, "任务文件不存在，开始新建。");
		inFile.peek();  // 查看下一个字符但不移动指针
		if (inFile.eof()) {
			std::cout << "文件为空" << std::endl;
			return;
		}
		return;
	}

	// 读取整个文件内容
	std::string jsonData((std::istreambuf_iterator<char>(inFile)),
		std::istreambuf_iterator<char>());

	// 关闭文件
	inFile.close();

	// 解析 JSON 数据
	json jsonDataParsed = json::parse(jsonData);
	// 遍历 JSON 数组中的每个任务对象
	for (const auto& item : jsonDataParsed) {
		Task task;
		task.id = item.at("id").get<int>();       // 如果 key 不存在或类型不对，会抛异常
		task.description = item.at("description").get<std::string>();
		task.priority = item.at("priority").get<int>();
		task.dueDate = item.at("dueDate").get<std::string>();

		tasks.push_back(task);
		Logger::GetInstance().log(LogLevel::INFO, "加载任务成功。");
		if (task.id >= next_Id) {
			next_Id = task.id + 1;
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
	Logger::GetInstance().log(LogLevel::INFO, "添加任务成功。", "task add ", task.toString());
	saveTasks();//保存任务到文件
}

void TaskManager::saveTasks() const
{
	std::ofstream outFile("tasks.json");
	if (!outFile) {
		Logger::GetInstance().log(LogLevel::ERROR, "无法打开任务文件进行保存。");
		return;
	}

	try {
		json jsonData = json::array();

		for (const auto& task : tasks) {
			json taskJson;
			taskJson["id"] = task.id;
			taskJson["description"] = task.description;
			taskJson["priority"] = task.priority;
			taskJson["dueDate"] = task.dueDate;

			jsonData.push_back(taskJson);
		}

		outFile << jsonData.dump(4); // 美化格式
	}
	catch (const std::exception& e) {
		Logger::GetInstance().log(LogLevel::ERROR, "保存任务时发生异常: ", e.what());
	}

	outFile.close();
	Logger::GetInstance().log(LogLevel::INFO, "保存任务成功。");
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
			Logger::GetInstance().log(LogLevel::INFO, "更新前任务: " + task.toString());
			task.description = description;
			task.priority = priority;
			task.dueDate = dueDate;
			Logger::GetInstance().log(LogLevel::INFO, "更新后任务: " + task.toString());
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
	std::cout << it->toString() << std::endl;
	if (it != tasks.end()) {
		std::string taskInfo = it->toString();
		tasks.erase(it);
		Logger::GetInstance().log(LogLevel::INFO, "删除任务: " + taskInfo);
		saveTasks();
	}
	else {
		std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
	}
}

void TaskManager::upgrate_index() {
	next_Id = 1;
	for (auto& task : tasks)
	{
		task.id = next_Id++;
		saveTasks();
	}
	return;
}

bool TaskManager::compareByPriority(const Task& a, const Task& b) {
	return a.priority < b.priority;
}

bool TaskManager::compareByDueDate(const Task& a, const Task& b) {
	return a.dueDate < b.dueDate;
}



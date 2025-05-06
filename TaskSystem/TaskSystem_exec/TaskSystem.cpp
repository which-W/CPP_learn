// TaskSystem.cpp: 定义应用程序的入口点。
//

#include "TaskSystem.h"
#include "TaskManager.h"
#include "Logger.h"
#include "global.h"
#include "Command.h"
int main()
{
	TaskManager taskManager;

	AddCommand addCommand(taskManager);
	addCommand.execute("测试任务,1,2023-10-01");
	return 0;
}

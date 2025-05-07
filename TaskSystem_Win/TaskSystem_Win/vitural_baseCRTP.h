#pragma once
//1,虚函数构建
	//AddCommand addCmd(taskManager);
	//addCmd.execute("test,1,1-1-1");
	//DeleteCommand delCmd(taskManager);
	//ListCommand listCmd(taskManager);
	//UpdateCommand updateCmd(taskManager);
	////使用unique来避免复制
	//std::unordered_map<std::string, std::unique_ptr<CommandBase>> commandMap;
	//commandMap["add"] = std::make_unique<AddCommand>(taskManager);
	//commandMap["delete"] = std::make_unique<DeleteCommand>(taskManager);
	//commandMap["list"] = std::make_unique<ListCommand>(taskManager);
	//commandMap["update"] = std::make_unique<UpdateCommand>(taskManager);
	//commandMap["upgrate_index"] = std::make_unique<Update_IndexCommand>(taskManager);

//1, 类型擦除
	//auto add_command = std::make_shared<AddCommand>(taskManager);
	//auto delete_command = std::make_shared<DeleteCommand>(taskManager);
	//auto list_command = std::make_shared<ListCommand>(taskManager);
	//auto update_command = std::make_shared<UpdateCommand>(taskManager);
	//auto upgrate_index = std::make_shared<Update_IndexCommand>(taskManager);
	//std::unordered_map<std::string, std::function<void(const std::string&)> > commandMap;
	////放入map
	//commandMap["add"] = [add_command](const std::string& args)
	//	{ add_command->execute(args); };
	//commandMap["delete"] = [delete_command](const std::string& args)
	//	{ delete_command->execute(args); };
	//commandMap["list"] = [list_command](const std::string& args)
	//	{ list_command->execute(args); };
	//commandMap["update"] = [update_command](const std::string& args)
	//	{ update_command->execute(args); };
	//commandMap["upgrate_index"] = [upgrate_index](const std::string& args)
	//	{ upgrate_index->execute(args); };
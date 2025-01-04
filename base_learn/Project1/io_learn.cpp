#include<iostream>
#include<string>
int main() {
	int i;
	while (std::cin>>i ,!std::cin.eof() ) {
		//判断系统内部的Cin出错
		if (std::cin.bad()) {
			throw std::runtime_error("cin is corrupted");
		}
		//判断输入格式错误
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		std::cout << i << std::endl;
	}
	std::cout << "process compeleted" << std::endl;
	//getline函数的使用
	std::string i2;
	while (std::getline(std::cin,i2) , !std::cin.eof())
	{
		if (std::cin.bad()) {
			throw std::runtime_error("cin is corrupted");
		}
		std::cout << i2 << std::endl;
	}
	//get的使用
	char i3;
	while (std::cin.get(i3), !std::cin.eof())
	{
		if (std::cin.bad()) {
			throw std::runtime_error("cin is corrupted");
		}
		std::cout << i3 << std::endl;
	}
	
	return 0;
}
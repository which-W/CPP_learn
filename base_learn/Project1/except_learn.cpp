#include<iostream>
//比较标准的除法处理
int Divide(int devision, int devisor) {
	if (!devisor) {
		throw std::string("devior is zero");
	}
	return devision / devisor;
}
void imputnumber(const std::string& str, int& num) {
	std::cout << str << std::endl;
	while (std::cin >> num, !std::cin.eof())
	{
		if (std::cin.bad()) {
			throw std::runtime_error("cin is corrupted");
		}
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "num format is worry" << std::endl;
			continue;
		}
		break;
	}
	
}

int main() {
	int devision = 0;
	int devisor = 0;
	imputnumber("input a devision", devision);
	imputnumber("input a adevisor", devisor);
	try
	{
		std::cout << Divide(devision, devisor) << std::endl;
	}
	//这里的except接受的的是throw的内容
	catch (const std::string& except)
	{
		std::cout << "exception error : exception is "<< except << std::endl;
	}
	catch (...) {
		std::cout << "exception error : exception is unknow" << std::endl;
	}
	



	return 0;

}
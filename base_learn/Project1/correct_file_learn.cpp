#include<iostream>
#include<fstream>

//���ļ�����ȷ�Ĵ���ʽ
void outputfile(const std::string filename) {
	std::ifstream ifs(filename);
	std::string filecontent;
	if (ifs.is_open()) {
		while (ifs >> filecontent)
		{
			std::cout << filecontent << std::endl;
			if (ifs.bad()) {
				throw std::runtime_error(" ifs is coruption");
			}
			ifs.close();
		}
	
	}
	else {
		if (ifs.bad()) {
			throw std::runtime_error(" ifs is coruption");
		}
		else  {
			if (ifs.fail()) {
				throw std::string("ifsname is worry");
			}
		}
	}
}

int main() {
	std::string str;
	std::cin >> str;
	try
	{
		outputfile(str);
	}
	catch (const std::string & except)
	{
		std::cout << "exception is" << except <<std::endl;
	}catch(...){
		std::cout << "exception is unknow" << std::endl;
	}
}
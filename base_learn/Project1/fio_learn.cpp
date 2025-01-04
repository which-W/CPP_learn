#include<iostream>
#include<fstream>
#include<string>

int main() {
	//在对fstream实例化时有不带参数的，也可以在初始时就打开文件,也可以加上文件的格式在后面。
	//写入
	std::ifstream fs1;
	//写出
	std::ofstream fs2;
	//既能写入又能写出
	std::fstream fs3;
	std::string fsname1;
	std::ifstream fs1(fsname1);
	std::ofstream fs2(fsname1);
	std::fstream fs3(fsname1);


	//一个文件读取的范例
	std::string fsname;
	std::string fscontent;
	while (std::cin>>fsname , !std::cin.eof())
	{
		if (std::cin.bad()){
			throw std::runtime_error("cin is corrupted");
		}
		std::ifstream fs(fsname);
		if (fs.is_open()) {
			while (std::getline(fs, fscontent)) {
				std::cout << fscontent << std::endl;
			}
			if (fs.bad()) {
			throw std::runtime_error("fs is corrupted");
			};
			fs.close();
		}
		else {
			fs.clear();
			fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
	}
	//写文件ostream其中加上app是追加模式，默认模式是将第一行数据改成输入的数据。
	std::ofstream ofs("name.txt", std::ios::app);
	if (ofs.is_open()) {
		ofs << "hello world"<<std::endl;
		ofs.close();
	}
}
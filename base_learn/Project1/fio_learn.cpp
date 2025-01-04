#include<iostream>
#include<fstream>
#include<string>

int main() {
	//�ڶ�fstreamʵ����ʱ�в��������ģ�Ҳ�����ڳ�ʼʱ�ʹ��ļ�,Ҳ���Լ����ļ��ĸ�ʽ�ں��档
	//д��
	std::ifstream fs1;
	//д��
	std::ofstream fs2;
	//����д������д��
	std::fstream fs3;
	std::string fsname1;
	std::ifstream fs1(fsname1);
	std::ofstream fs2(fsname1);
	std::fstream fs3(fsname1);


	//һ���ļ���ȡ�ķ���
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
	//д�ļ�ostream���м���app��׷��ģʽ��Ĭ��ģʽ�ǽ���һ�����ݸĳ���������ݡ�
	std::ofstream ofs("name.txt", std::ios::app);
	if (ofs.is_open()) {
		ofs << "hello world"<<std::endl;
		ofs.close();
	}
}
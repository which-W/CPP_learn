#include<iostream>
#include<string>
#include<sstream>
//sstream��Ҫ������������Ϊ��Ҫ��
int main() {
	//string������Ҫ�Ĺ��ܰ��ַ����ĳ�����
	std::string str = "12";
	std::stringstream strstream(str);
	int i = 0;
	strstream >> i;
	if (strstream.bad()) {
		throw std::runtime_error("strstream is corrupted");
	}
	else if(strstream.fail())
	{
		strstream.clear();
		strstream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "form is wrong" << std::endl;
	}
	std::cout << i << std::endl;
	return 0;
	//Ҳ���԰�Intת����string����
    int i2 = 12;
	std::stringstream strstream1(i2);
	strstream1 << i2 << std::endl;
	if (strstream1.bad()) {
		throw std::runtime_error("strstream is corrupted");
	}
	else {
		std::cout << strstream1.str() << std::endl;
	}

	//���ַ���֮��Ŀո�ȥ��
	std::string strstring("hello world everyone");
	std::string destring;
	std::stringstream strstream(strstring);
	while (strstream >> destring)
	{
		std::cout << destring << std::endl;
	}
	if (strstream.bad()) {
		throw std::runtime_error("strstream is corrupted");
	}
}
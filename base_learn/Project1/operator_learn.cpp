#include<iostream>
#include<vector>
class Test {
public:
	friend std::ostream& operator<< (std::ostream& os, const Test& test);
	friend std::istream& operator>> (std::istream& is, Test& test);
	//һԪ����������ز���
	void operator ++ () {
		count++;
	}
	void operator --() {
		count--;
	}
	int operator[](unsigned i) const{
		if (i > 0 && i < ivec.size()) {
			return ivec[i];
		}
	}
	void operator()() const{
		std::cout << "use this function" << std::endl;
	}
	void operator()(const std::string s1) {
		std::cout << s1 << std::endl;
	}
//��Ԫ�����������,�ͼӺ���ͬ��д��
	Test operator + (const Test & test) {
		count = count + test.count;
		return *this;
	}
//���ڲ�̫һ��,һ�����ϵͳ��Ĭ���������Կ��Բ����Լ�����
	Test& operator = (Test& test) {
		if (this == &test) {
			return *this;
		}
		this->name = test.name;
		this->count = test.count;
		return *this;
	}
	
private:
		int count;
		std::vector<int> ivec{ 1,2,3,4,5,6 };
		std::string name;
};
std::istream& operator>> (std::istream& is, Test& test) {
	is >> test.name;
	return is;
}
std::ostream& operator<< (std::ostream& os, const Test& test) {
	os << test.name << std::endl;
	return os;
}
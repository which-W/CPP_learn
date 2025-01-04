#include<iostream>
#include<vector>
class Test {
public:
	friend std::ostream& operator<< (std::ostream& os, const Test& test);
	friend std::istream& operator>> (std::istream& is, Test& test);
	//一元运算符的重载操作
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
//二元运算符的重载,和加号相同的写法
	Test operator + (const Test & test) {
		count = count + test.count;
		return *this;
	}
//等于不太一样,一般操作系统会默认重载所以可以不用自己重载
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
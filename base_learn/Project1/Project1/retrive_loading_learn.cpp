#include<iostream>
#include<vector>

class Test
{
public:
	friend std::iostream& operator >>(std::iostream&is, Test& test);
	friend std::ostream& operator <<(std::ostream&os, const Test& test);

	void operator--(){
		count--;
	}
	void operator++() {
		count++;
	}
	unsigned operator[](int i) {
		if (i > 0 && i < array.size()) {
			return array[i];
		}
		else {
			return -1;
		}
	}
	Test operator+(const Test& test) {
		count = test.count;
		return *this;
	}

	bool operator>(const Test& test) {
		return count > test.count;
	}


	std::vector<int> array{ 1,2,3,4,5 };
	std::string name;
	int count = 0;
};

std::istream& operator >>(std::istream&is, Test& test) {
	is >> test.name;
	return is;
};
std::ostream& operator <<(std::ostream& os, const Test& test) {
	os << test.name << std::endl;
	return os;
};


Test::~Test()
{
}
int main() {
	Test test;
	++test;
	--test;
	test[4];
	std::cin >>test;
	std::cout << test;

	return 0;
}
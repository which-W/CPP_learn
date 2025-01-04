#include<iostream>
using namespace std;
//父类
//使用虚函数时父类的析构函数必须是虚函数不然会导致子类的内存泄露
class Spear {
public:
	Spear(const string& name_, const string& icon_) :name(name_), icon(icon_) {};
	virtual void openfire()const {
		cout << "Spear : fire" << endl;
	}
protected:
	string name;
	string icon;
};

class fireSpear : public Spear {
public:
	fireSpear(const string& name_, const string& icon_, const int i_) :Spear(name_, icon_), i(i_) {};
	//override可以标记虚函数防止写错
	virtual void openfire()const override {
		cout << "fireSpear : fire" << endl;
	}
private:
	int i;
};
void openfire(const Spear* ispear) {
	ispear->openfire();
	delete ispear;
}
//如果没有家Virtual这个前置的关键字那么openfire就会调用父类的函数如果有那么就调用子类的,当然只有父类有效果一样

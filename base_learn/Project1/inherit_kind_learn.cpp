#include<iostream>
using namespace std;
//父类
class Spear {
public:
	Spear(const string& name_, const string& icon_) :name(name_), icon(icon_) {};
protected:
	string name;
	string icon;
};
//子类
class fireSpear : public Spear {
public:
	fireSpear(const string& name_, const string& icon_, const int i_) :Spear(name_, icon_), i(i_) {};
private:
	int i;
};

class iceSpear : public Spear {

};
//父类的指针可以直接指向子类如：Spear * isspear = new fireSpear();
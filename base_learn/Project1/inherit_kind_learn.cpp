#include<iostream>
using namespace std;
//����
class Spear {
public:
	Spear(const string& name_, const string& icon_) :name(name_), icon(icon_) {};
protected:
	string name;
	string icon;
};
//����
class fireSpear : public Spear {
public:
	fireSpear(const string& name_, const string& icon_, const int i_) :Spear(name_, icon_), i(i_) {};
private:
	int i;
};

class iceSpear : public Spear {

};
//�����ָ�����ֱ��ָ�������磺Spear * isspear = new fireSpear();
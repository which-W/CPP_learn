#include<iostream>
using namespace std;
//����
//ʹ���麯��ʱ��������������������麯����Ȼ�ᵼ��������ڴ�й¶
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
	//override���Ա���麯����ֹд��
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
//���û�м�Virtual���ǰ�õĹؼ�����ôopenfire�ͻ���ø���ĺ����������ô�͵��������,��Ȼֻ�и�����Ч��һ��

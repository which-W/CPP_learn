#include<iostream>

class MyClass
{
public:
	static unsigned i;

};	

unsigned MyClass::i = 20;

//��̬��Ա���������������ʼ�����ҳ�ʼ�������ڹ��캯��֮�У�����ֱ�����������þ�̬��Ա���������������

//��̬��Ա����Ϊ�˱�����ķ�װ�Զ�����������

class MyClass2
{
public:
	static unsigned getI() { return i; }
private:
	static unsigned i;

};

unsigned MyClass2::i = 20;
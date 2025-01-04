#include<iostream>

class MyClass
{
public:
	static unsigned i;

};	

unsigned MyClass::i = 20;

//静态成员变量必须在类外初始化并且初始化不能在构造函数之中，可以直接用类来调用静态成员变量而不用类对象

//静态成员函数为了保护类的封装性而产生的例如

class MyClass2
{
public:
	static unsigned getI() { return i; }
private:
	static unsigned i;

};

unsigned MyClass2::i = 20;
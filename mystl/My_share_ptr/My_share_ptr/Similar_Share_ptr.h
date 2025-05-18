#pragma once
#include<atomic>
#include<mutex>
struct Controlcount
{
	std::atomic<int> ref_count;
	Controlcount(); //默认引用计数为1
	std::mutex m_mutex; //互斥锁,暂时可以不用
};

template<typename T>
class Similar_Share_ptr
{
public:
	//explicit防止隐式转换-> Similar_Share_ptr<int> p = new Student(),只能通过Similar_Share_ptr<int> p(new Student())来创建对象
	explicit Similar_Share_ptr(T* p) :ptr(p);
	~Similar_Share_ptr();
	//拷贝构造函数,移动构造函数，拷贝赋值函数，移动赋值函数
	Similar_Share_ptr(const Similar_Share_ptr& other);
	Similar_Share_ptr& operator = (const Similar_Share_ptr& other) {
		if (this != &other) {
			release();
			ptr = other.ptr;
			control = other.control;
			if (control) {
				control->ref_count++;
			}
		}
		return *this;
	}
	Similar_Share_ptr(Similar_Share_ptr&& other) noexcept;
	Similar_Share_ptr& operator = (Similar_Share_ptr&& other) noexcept {
		if (this != &other) {
			release();
			ptr = other.ptr;
			control = other.control;
			other.ptr = nullptr;
			other.control = nullptr;
		}
		return *this;
	}
	//重载*和->运算符实现类的指针调用方法
	//Similar_Share_ptr<Student> p(new Student()); p->getName()=p.ptr->getName();
	T* operator->() const;
	//重载*运算符实现类的指针调用方法
	//Similar_Share_ptr<Student> p(new Student());(*p)->getName()
	T& operator*() const;
	//返回央指针
	T* get() const;
	//返回引用计数
	int use_count() const;
	
	void reset(T* p = nullptr);
private:
	T* ptr;
	Controlcount* control;
	void release();
};


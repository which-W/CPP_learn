#pragma once
#include<atomic>
#include<mutex>
struct Controlcount
{
	std::atomic<int> ref_count;
	Controlcount(); //Ĭ�����ü���Ϊ1
	std::mutex m_mutex; //������,��ʱ���Բ���
};

template<typename T>
class Similar_Share_ptr
{
public:
	//explicit��ֹ��ʽת��-> Similar_Share_ptr<int> p = new Student(),ֻ��ͨ��Similar_Share_ptr<int> p(new Student())����������
	explicit Similar_Share_ptr(T* p) :ptr(p);
	~Similar_Share_ptr();
	//�������캯��,�ƶ����캯����������ֵ�������ƶ���ֵ����
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
	//����*��->�����ʵ�����ָ����÷���
	//Similar_Share_ptr<Student> p(new Student()); p->getName()=p.ptr->getName();
	T* operator->() const;
	//����*�����ʵ�����ָ����÷���
	//Similar_Share_ptr<Student> p(new Student());(*p)->getName()
	T& operator*() const;
	//������ָ��
	T* get() const;
	//�������ü���
	int use_count() const;
	
	void reset(T* p = nullptr);
private:
	T* ptr;
	Controlcount* control;
	void release();
};


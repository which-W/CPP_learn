#include "Similar_Share_ptr.h"

Controlcount::Controlcount() : ref_count(1) {} //默认引用计数为1

template<typename T>
Similar_Share_ptr<T>::Similar_Share_ptr(T* p) : ptr(p) {
	if (p) {
		control = new Controlcount();
	}
	else
	{
		control = nullptr;
	}
}

template<typename T>
Similar_Share_ptr<T>::~Similar_Share_ptr() {
	if (ptr) {
		release();
	}
}

template<typename T>
void Similar_Share_ptr<T>::release() {
	if (control) {
		control->ref_count--;
		if (control->ref_count == 0) {
			delete control;
			delete ptr;
			control = nullptr;
			ptr = nullptr;
		}
	}
	else
	{
		delete control;
	}
}

template<typename T>
Similar_Share_ptr<T>::Similar_Share_ptr(const Similar_Share_ptr& other) : ptr(other.ptr), control(other.control) {
	if (control) {
		control->ref_count++;
	}
}

template<typename T>
Similar_Share_ptr<T>::Similar_Share_ptr(Similar_Share_ptr&& other) noexcept : ptr(other.ptr), control(other.control) {
	other.ptr = nullptr;
	other.control = nullptr;
}

template<typename T>
T* Similar_Share_ptr<T>::operator->() const {
	return ptr;
}
template<typename T>
T& Similar_Share_ptr<T>::operator*() const {
	return *ptr;
}
template<typename T>
T* Similar_Share_ptr<T>::get() const {
	return ptr;
}
template<typename T>
int Similar_Share_ptr<T>::use_count() const {
	if (control) {
		return control->ref_count.load();
	}
	else
	{
		return 0;
	}
}

template<typename T>
void Similar_Share_ptr<T>::reset(T* p) {
	if (ptr != p) {
		release();
		ptr = p;
		if (p) {
			control = new Controlcount();
		}
		else
		{
			control = nullptr;
		}
	}
}
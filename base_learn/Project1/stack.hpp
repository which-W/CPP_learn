#include<iostream>
#include<stdexcept>

using namespace std;
//顺序表实现栈和顺序表基本一样只是一个从前一个从后
template<typename T>
class stack
{
public:
	stack() : data(new T[capacity]), size(0), capacity(10) {}
	~stack();
	void push(T element);
	T pop();
	T top() const;
	int getsize() const;


private:
	T* data;
	int size;
	int capacity;
	void resize();

};

template<typename T>
void stack<T>::resize() {
	int newcapacity = capacity * 2;
	T* newdata = new T[newcapacity];
	for (size_t i = 0; i < size; i++) {
		newdata[i] = data[i];
	}
	delete[] data;
	newdata = data;
	capacity = newcapacity;
}

template<typename T>
stack<T>::~stack()
{
	delete[] data;
}

template<typename T>
void stack<T>::push(T element) {
	if (size == capacity) {
		resize();
	}
	data[size++] = element;
}

template<typename T>
T stack<T>::pop() {
	if (size == 0) {
		throw std::underflow_error("stack is empty");
	}
	return data[--size];
}

template<typename T>
T stack<T>::top() const {
	if (size == 0) {
		throw std::underflow_error("stack is empty");
	}
	return data[size - 1];
}
template<typename T>
int stack<T>::getsize() const {
	return size;
}
//通过链表实现栈
template<typename T>
class Stask2 {
private:
	struct Node
	{
		T data;
		Node* next;
		Node(T data_) :data(data_), next(NULL) {};
	};
	Node* head;
	int size;
public:
	Stask2() :head(NULL), size(0) {};
	~Stask2();
	void push(T element);
	T pop();
	T top()const;
	int getsize() const;
};

template<typename T>
Stask2<T>::~Stask2() {
	while (head)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}

template<typename T>
void Stask2<T>::push(T element) {
	Node* newnode = new Node(element);
	newnode->next = head;
	newnode = head;
	size++;
}

template<typename T>
T Stask2<T>::pop() {
	if (head == NULL) {
		throw std::underflow_error("stack is empty");
	}
	T result = head->data;
	Node* temp = head;
	head = head->next;
	delete temp;
	return result;
}

template<typename T>
T Stask2<T>::top()const {
	if (head == NULL) {
		throw std::underflow_error("stack is empty");
	}
	T result = head->data;
	return result;
};

template<typename T>
int Stask2<T>::getsize()const {
	return size;
}

//进制转换问题可以用栈来解决
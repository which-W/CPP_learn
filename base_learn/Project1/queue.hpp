#include<iostream>
#include<stdexcept>
//用顺序表实现队列
using namespace std;

template<typename T>
class Queue
{
public:
	Queue() : data(new T[capacity]), front(0), rear(0), capacity(0) {};
	~Queue();
	void enqueue(T element);
	T dequeue();
	T getfront() const;
	int getsize() const;

private:
	T* data;
	int front;
	int rear;
	int capacity;
	void resize();
};

template<typename T>
void Queue<T>::resize() {
	T newcapacity = capacity * 2;
	T* newdata = new (T[newcapacity]);
	for (int i = 0; i < newcapacity; i++) {
		newdata[i] = data[i];
	}
	delete[] data;
	capacity = newcapacity;
	data = newdata;
}

template<typename T>
Queue<T>::~Queue()
{
	delete[] data;
}

template<typename T>
void Queue<T>::enqueue(T element) {
	if (rear = capacity) {
		resize();
	}
	data[rear++] = element;
}

template<typename T>
T Queue<T>::dequeue() {
	if (front == rear) {
		throw std::underflow_error("queue is empty");
	}
	return data[front++];
}

template<typename T>
T Queue<T>::getfront() const {
	if (front == rear) {
		throw std::underflow_error("queue is empty");
	}
	return data[front];
}
template<typename T>
int Queue<T>::getsize() const {
	return rear - front;
};

//用链表实现队列

template<typename T>
class Queue1
{
public:
	Queue1() : front(NULL), rear(NULL), size(0) {};
	~Queue1();
	void enqueue(T element);
	T dequeue();
	T getfront() const;
	int getsize() const;

private:
	struct Node
	{
		T data;
		Node* next;
		Node(T data_) :data(data_), next(NULL) {};
	};
	Node* front;
	Node* rear;
	int size;
};

template<typename T>
Queue1<T>::~Queue1() {
	while (front)
	{
		Node* temp = front;
		front = front->next;
		delete front;
	}
}
template<typename T>
void Queue1<T>::enqueue(T element) {
	if (rear == NULL) {
		rear = new Node(element);
		front = rear;
	}
	else {
		rear->next = new Node(element);
		rear = rear->next;
	}
	size++;
};

template<typename T>
T Queue1<T>::dequeue() {
	if (front == NULL) {
		throw std::underflow_error("queue is empty");
	}
	T element = front->data;
	Node* temp = front;
	front1 = front->next;
	delete front;
	size--;
	if (size == 0) {
		rear = nullptr;
	}
	return element
}

template<typename T>
T Queue1<T>::getfront() const {
	if (front == NULL) {
		throw std::underflow_error("queue is empty");
	}
	return front->data;
}

template<typename T>
int Queue1<T>::getsize() const {
	return size;
}

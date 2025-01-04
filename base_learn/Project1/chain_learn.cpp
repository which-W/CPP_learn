//链表的实现
#include<iostream>
#include<stdexcept>

using namespace std;
#define eletype int
struct Listnode {
	eletype data;
	Listnode* next;
	Listnode(eletype i) :data(i), next(NULL) {};
};

class Linklist {
private:
	Listnode* head;
	int size;
public:
	Linklist() :head(NULL), size(0) {};
	~Linklist();
	void insert(int i, eletype data);
	void remove(int i);
	Listnode* find(eletype data);
	Listnode* get(int i);
	void update(int i, eletype data);
	void print();
};

Linklist::~Linklist() {
	Listnode* cur = head;
	while (!cur) {
		Listnode* temp = cur;
		cur = cur->next;
		delete temp;
	}
}

void Linklist::insert(int i, eletype data) {
	if (i < 0 || i >size) {
		throw std::out_of_range("index is null");
	}
	Listnode* Newnode = new Listnode(data);
	if (i == 0) {
		Newnode->next = head;
		head = Newnode;
	}
	else {
		Listnode* cur = head;
		for (int j = 0; j < i - 1; j++) {
			cur = cur->next;
		}
		Newnode->next = cur->next;
		cur->next = Newnode;
	}
	size++;
}
void Linklist::remove(int i) {
	if (i < 0 || i >size) {
		throw std::out_of_range("index is null");
	}
	if (i == 0) {
		Listnode* temp = head;
		head = head->next;
		delete temp;
	}
	else {
		Listnode* cur = head;
		for (int j = 0; j < i - 1; j++) {
			cur = cur->next;
		}
		Listnode* temp = cur->next;
		cur->next = temp->next;
		delete temp;
	}
	size--;
}

Listnode* Linklist::find(eletype data) {
	Listnode* cur = head;
	while (cur != NULL && cur->data != data) {
		cur = cur->next;
	}
	return cur;
};
Listnode* Linklist::get(int i) {
	if (i < 0 || i >size) {
		throw std::out_of_range("index is null");
	}
	Listnode* cur = head;
	for (int j = 0; j < i - 1; j++) {
		cur = cur->next;
	}
	return cur->next;
};

void Linklist::update(int i, eletype data) {
	if (i < 0 || i >size) {
		throw std::out_of_range("index is null");
	}
	Listnode* cur = head;
	for (int j = 0; j < i; j++) {
		cur = cur->next;
	}
	cur->data = data;
};

void Linklist::print() {
	Listnode* cur = head;
	while (cur)
	{
		cout << cur->data << endl;
		cur = cur->next;
	}
	cout << endl;
};
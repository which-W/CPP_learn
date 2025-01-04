#include<iostream>

using namespace std;
template<typename T>
struct Listnode
{
	T data;
	Listnode* next;
	Listnode(T d) :data(d), next(nullptr) {};
};

template<typename T>
struct Treenode
{
	T data;
	Listnode<Treenode<T>*>* childhead;
	Treenode() {
		childhead = NULL;
	}
	void addchild(Treenode<T> node) {
		Listnode<Treenode<T>*>* childnode = new Listnode<Treenode<T> *> node;
		if (childhead) {
			childnode->next = childhead;
			childhead = childnode;
		}
		else {
			childhead = childnode;
		}
	}

};

template<typename T>
class tree
{
public:
	tree();
	tree( int max_node);
	~tree();
	Treenode<T>* gettreenode(int id);
	void setroot(int rootid);
	void addchild(int parentid, int sonid);
	void assigndata(int nodeid, T data);
	void print(Treenode<T>* node = NULL);

private:
	Treenode<T>* nodes;
	Treenode<T>* root;

};
template<typename T>
tree<T>::tree()
{
	nodes = new Treenode<T>[10001];
}

template<typename T>
tree<T>::tree(int max_node)
{
	nodes = new Treenode<T>[max_node];
}
template<typename T>
tree<T>::~tree()
{
	delete[] nodes;
}

template<typename T>
Treenode<T>* tree<T>::gettreenode(int id) {
	return &nodes[id];
};
template<typename T>
void tree<T>::setroot(int rootid) {
	root = gettreenode(id);
};

//给节点加边链接起来
template<typename T>
void tree<T>::addchild(int parentid, int sonid) {
	Treenode<T>* parentNode = gettreenode(parentid);
	Treenode<T>* childNode = gettreenode(sonid);
	parentNode->addchild(childNode);
};


template<typename T>
void tree<T>::assigndata(int nodeid, T data) {
	gettreenode(nodeid)->data = data;
};

//树的深度优先遍历
template<typename T>
void tree<T>::print(Treenode<T>* node) {
	if (node == NULL) {
		root = node;
	}
	cout << node->data;
	Listnode<Treenode<T>*>* temp = node->childhead;
	while (temp) {
		print(temp->data);
		temp = temp->next;
	}
};
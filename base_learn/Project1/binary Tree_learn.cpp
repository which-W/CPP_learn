#include<iostream>
using namespace std;

template<typename T>
struct treenode {
	T data;
	treenode* left;
	treenode* right;
	treenode():data(0),left(nullptr),right(nullptr){}
	treenode(T data_):data(data_), left(nullptr), right(nullptr) {}
};


template<typename T>
class Tree
{
private:
	treenode<T>* nodes;
	treenode<T>* root;
	size_t nodeSize;
	//创建树
	treenode<T>create(T a[], size_t size, int nodeID, T Nullnode);
	void visit(treenode<T>nodes);
	//按前序中序后序组建树的节点
	void preorder(treenode<T>nodes);
	void inorder(treenode<T>nodes);
	void postorder(treenode<T>nodes);

public:
	Tree();
	Tree(int maxnodesID);
	~Tree();
	treenode<T> * Getnode(int id);
	void Createtree(T a[], size_t size, T nodeNull);
	void preorderTraverral();
	void inorderTraverral();
	void postorderTraverral();

};



template<typename T>
Tree<T>::Tree()
{
	nodeSize = 1000001;
	nodes = new treenode<T>[nodesize];
}
template<typename T>
Tree<T>::Tree(int maxnodessize) {
	nodeSize  = maxnodessize
	nodes = new treenode<T>[nodeSize];
};

template<typename T>
Tree<T>::~Tree()
{
	delete[] nodes;
}
template<typename T>
treenode<T>* Tree<T>::Getnode(int id) {
	return &nodes[id];
};

template<typename T>
void Tree<T>::visit(treenode<T>nodes) {
	cout << nodes->data << endl;
};


template<typename T>
void Tree<T>::preorder(treenode<T>node) {
	if (node) {
		visit(node);
		preorder(node->left);
		preorder(node->right);
	}
};


template<typename T>
void Tree<T>::inorder(treenode<T>node) {
	if (node) {
		preorder(node->left);
		visit(node);
		preorder(node->right);
	}
}

template<typename T>
void Tree<T>::postorder(treenode<T>node) {
	if (node) {
		preorder(node->left);
		preorder(node->right);
		visit(node);
	}
}
//这个是调用的接口
template<typename T>
void Tree<T>::Createtree(T a[], size_t size, T nodeNull) {
	root = creat(a, size, 1, nodeNull);
}
//这个是内部实现的函数
template<typename T>
treenode<T> Tree<T>::create(T a[], size_t size, int nodeID, T Nullnode) {
	if (nodeID > size || a[nodeID] == Nullnode) {
		return NULL;
	}
	treenode<T>* newnodes = Getnode(nodeID);
	newnodes->data = nodes[nodeID];
	newnodes->left = create(a, size, nodeID * 2, Nullnode);
	newnodes->right = create(a, size, nodeID * 2 + 1, Nullnode);
	return newnodes;
};


template<typename T>
void Tree<T>::preorderTraverral() {
	preorder(root);
};

template<typename T>
void Tree<T>::inorderTraverral() {
	inorder(root);
};


template<typename T>
void Tree<T>::postorderTraverral() {
	postorder(root);
};
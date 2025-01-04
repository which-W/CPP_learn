#include<iostream>

template<typename T>
struct Treenode
{
	T val;
	Treenode<T>* left;
	Treenode<T>* right;
	Treenode() :val(0), left(nullptr), right(nullptr) {};
	Treenode(T val_) :val(val_), left(nullptr), right(nullptr) {};
};

template<typename T>
class BinarySearchTree
{
private:
	Treenode<T>* root;

	Treenode<T>* insertTreenode(Treenode<T>* node, T val);
	Treenode<T>* removeTreenode(Treenode<T>* node, T val);
	bool findTreenode(Treenode<T>* node, T val);
	void inorder(Treenode<T>* node);

public:
	BinarySearchTree() :root(NULL) {};
	~BinarySearchTree();

	void insert(T val) {
		root = insertTreenode(root, val);
	}

	void remove (T val) {
		root = removeTreenode(root, T val);
	}

	bool findnode(T val) {
		return findTreenode(root, T val);
	}

	void inorderSearch() {
		inorder(root);
	}

};
template<typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
	while (root)
	{
		remove(root->val);
	};
}

template<typename T>
Treenode<T>* BinarySearchTree<T>::insertTreenode(Treenode<T>* node, T val) {
	if (node == NULL) {
		return new Treenode<T> (val);
	}
	if (val >node->val) {
		node->right = insertTreenode(node->right, val);
	}else if(val < node ->val) {
		node->left = insertTreenode(node->left, val);
	}
	return root;
};

template<typename T>
Treenode<T>* BinarySearchTree<T>::removeTreenode(Treenode<T>* node, T val) {
	if (node == NULL) {
		return NULL;
	}
	if (val > node->val) {
		node->right = removeTreenode(node->right, val);
	}
	else if (val < node->val) {
		node->left = removeTreenode(node->left, val);
	}
	else {
		if (node->left == NULL && node->right == NULL) {
			delete node;
		}
		else if (node->left == NULL) {
			Treenode<T>* rightchild = node->right;
			delete node;
			return rightchild;
		}
		else if (node->right == NULL) {
			Treenode<T>* leftchild = node->left;
			delete node;
			return leftchild;
		}
		else {
			Treenode<T>templatement = node->right;
			while (templatement->left)
			{
				templatement = templatement->left;
			}
			node->val = templatement->val;
			node -> right = removeTreenode(node->right, templatement->val);
		}
	}
	return node;
};

template<typename T>
bool BinarySearchTree<T>::findTreenode(Treenode<T>* node, T val) {
	if (node == NULL) {
		return false;
	}
	if (val > node->val) {
		return findTreenode(node->right, val);
	}
	else if (val < node->val) {
		return findTreenode(node->left, val);
	}
	return true
};

template<typename T>
void BinarySearchTree<T>::inorder(Treenode<T>* node) {
	if (node) {
		inorder(node->left);
		std::cout << node->val << ',';
		inorder(node->right);
	}
};
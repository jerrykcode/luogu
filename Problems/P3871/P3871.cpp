#include <cstdio>
#include <cstdlib>
using namespace std;

template<typename T>
class MidNum {
public:
	MidNum();
	~MidNum();

	void add(T num);
	T mid();

private:

	typedef struct SBTNode {
		T num;
		struct SBTNode * left;
		struct SBTNode * right;
		size_t size;
		SBTNode(T num) : num(num), left(NULL), right(NULL), size(1) {}
	} * SBT;

	SBT left_rotate(SBT tree);
	SBT right_rotate(SBT tree);

	SBT maintain(SBT tree);
	SBT insert(SBT tree, T num);
	T rank(SBT tree, size_t size);
	
	void delete_tree(SBT tree);

private:
	SBT tree;
};

template<typename T>
MidNum<T>::MidNum() {
	tree = NULL;
}

template<typename T>
MidNum<T>::~MidNum() {
	delete_tree(tree);
}

template<typename T>
void MidNum<T>::add(T num) {
	tree = insert(tree, num);
}

template<typename T>
T MidNum<T>::mid() {
	size_t n = tree->size / 2;
	if (tree->size % 2) { //size为奇数
		return rank(tree, n); //n个数比中位数小
	}
	else { //size为偶数
		return rank(tree, n - 1); //n - 1个数比中位数小
	}
}

template<typename T>
typename MidNum<T>::SBT MidNum<T>::left_rotate(SBT tree) {
	SBT k = tree->right;
	tree->right = k->left;
	k->left = tree;
	k->size = tree->size;
	size_t left_size = tree->left ? tree->left->size : 0; //左子树有可能为NULL
	size_t right_size = tree->right ? tree->right->size : 0; //右子树有可能为NULL
	tree->size = left_size + right_size + 1;
	return k;
}

template<typename T>
typename MidNum<T>::SBT MidNum<T>::right_rotate(SBT tree) {
	SBT k = tree->left;
	tree->left = k->right;
	k->right = tree;
	k->size = tree->size;
	size_t left_size = tree->left ? tree->left->size : 0; //左子树有可能为NULL
	size_t right_size = tree->right ? tree->right->size : 0; //右子树有可能为NULL
	tree->size = left_size + right_size + 1;
	return k;
}

template<typename T>
typename MidNum<T>::SBT MidNum<T>::maintain(SBT tree) {
	if (tree == NULL) return NULL;
	SBT left = tree->left, right = tree->right;
	size_t left_size = left ? left->size : 0;
	size_t right_size = right ? right->size : 0;
	if (left && left->left && left->left->size > right_size) { //左子树的左子树大于右子树
		tree = right_rotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	else if (left && left->right && left->right->size > right_size) { //左子树的右子树大于右子树
		tree->left = left_rotate(tree->left);
		tree = right_rotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	else if (right && right->right && right->right->size > left_size) { //右子树的右子树大于左子树
		tree = left_rotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	else if (right && right->left && right->left->size > left_size) { //右子树的左子树大于左子树
		tree->right = right_rotate(tree->right);
		tree = left_rotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

template<typename T>
typename MidNum<T>::SBT MidNum<T>::insert(SBT tree, T num) {
	if (tree == NULL) return new SBTNode(num);
	tree->size++;
	if (num < tree->num) {
		tree->left = insert(tree->left, num);
	}
	else {
		tree->right = insert(tree->right, num);
	}
	tree = maintain(tree); //维护平衡
	return tree;
}

template<typename T>
T MidNum<T>::rank(SBT tree, size_t size) {
	if (tree == NULL) return 0;
	size_t left_size = tree->left ? tree->left->size : 0;
	if (left_size == size) {
		return tree->num;
	}
	else if (left_size > size) {
		return rank(tree->left, size);
	}
	else {		
		return rank(tree->right, size - left_size - 1);
	}
}

template<typename T>
void MidNum<T>::delete_tree(SBT tree) {
	if (tree == NULL) return;
	delete_tree(tree->left);
	delete_tree(tree->right);
	delete tree;
}

int main() {
	int n, m, num;
	scanf("%d", &n);
	MidNum<int> mid_num;
	size_t i;
	for (i = 0; i < n; i++) {
		scanf("%d", &num);
		mid_num.add(num);
	}
	scanf("%d", &m);
	char cmd[3];
	for (i = 0; i < m; i++) {
		scanf("%s", cmd);
		if (cmd[0] == 'a') {
			scanf("%d", &num);
			mid_num.add(num);
		}
		else {
			printf("%d\n", mid_num.mid());
		}
	}
	return 0;
}
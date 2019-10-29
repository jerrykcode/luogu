#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t key_num; //key出现次数
	size_t num_size; //树中插入了多少个数
	size_t node_size; //树的结点个数
	struct TNode *left, *right;
} *Tree;

size_t getKeyNum(Tree t) { return t ? t->key_num : 0; }
size_t getNumSize(Tree t) { return t ? t->num_size : 0; }
size_t getNodeSize(Tree t) { return t ? t->node_size : 0; }

Tree newTNode(int key); //返回新结点

Tree leftRotate(Tree t); //左旋
Tree rightRotate(Tree t); //右旋

//维护平衡
Tree maintain(Tree t);
Tree maintainLeft(Tree t);
Tree maintainRight(Tree t);

//插入key
Tree insert(Tree t, int key);

//删除内存
void deleteTree(Tree t);

size_t smaller_num_; //记录插入过程中有多少个数小于待插入数

int main() {
	int n, key;
	scanf("%d", &n);
	Tree t = NULL;
	for (int i = 0; i < n; i++) {
		scanf("%d", &key);
		smaller_num_ = 0;
		t = insert(t, key);
		if (i) putchar(' ');
		printf("%d", smaller_num_);
	}
	deleteTree(t);
	return 0;
}

Tree newTNode(int key) {
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->key = key;
	t->key_num = 1;
	t->num_size = 1;
	t->node_size = 1;
	t->left = t->right = NULL;
	return t;
}

Tree leftRotate(Tree t) {
	Tree k = t->right;
	t->right = k->left;
	k->left = t;
	k->num_size = t->num_size;
	k->node_size = t->node_size;
	t->num_size = getNumSize(t->left) + getNumSize(t->right) + t->key_num;
	t->node_size = getNodeSize(t->left) + getNodeSize(t->right) + 1;
	return k;
}

Tree rightRotate(Tree t) {
	Tree k = t->left;
	t->left = k->right;
	k->right = t;
	k->num_size = t->num_size;
	k->node_size = t->node_size;
	t->num_size = getNumSize(t->left) + getNumSize(t->right) + t->key_num;
	t->node_size = getNodeSize(t->left) + getNodeSize(t->right) + 1;
	return k;
}

Tree maintain(Tree t) {
	t = maintainLeft(t);
	return maintainRight(t);
}

Tree maintainLeft(Tree t) {
	//维护平衡，保证左子树的两个子树的node_size小于右子树的node_size
	if (t == NULL || t->left == NULL) return t;
	if (t->left->left && t->left->left->node_size > getNodeSize(t->right)) {
		t = rightRotate(t);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	if (t->left->right && t->left->right->node_size > getNodeSize(t->right)) {
		t->left = leftRotate(t->left);
		t = rightRotate(t);
		t->left = maintain(t->left);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	return t;
}

Tree maintainRight(Tree t) {
	//维护平衡，保证右子树的两个子树的node_size小于左子树的node_size
	if (t == NULL || t->right == NULL) return t;
	if (t->right->right && t->right->right->node_size > getNodeSize(t->left)) {
		t = leftRotate(t);
		t->left = maintain(t->left);
		t = maintain(t);
	}
	if (t->right->left && t->right->left->node_size > getNodeSize(t->left)) {
		t->right = rightRotate(t->right);
		t = leftRotate(t);
		t->left = maintain(t->left);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	return t;
}

Tree insert(Tree t, int key) {
	if (t == NULL) {
		return newTNode(key);
	}
	t->num_size++; //只要插入数，num_size就增1
	if (key == t->key) { //key之前已插入过
		t->key_num++;
		smaller_num_ += getNumSize(t->left); //左子树的所有结点的key小于key，注意getNumSize不是getNodeSize
	}
	else if (key < t->key) {		
		size_t tmp_size = getNodeSize(t->left); //记录左子树的node_size
		t->left = insert(t->left, key); //递归插入左边
		if (getNodeSize(t->left) > tmp_size) { //若左子树的node_size变化，即左子树中新建了结点
			t->node_size++;
			t = maintainLeft(t); //维护平衡
		}
	}
	else { //key > t->key
		smaller_num_ += getNumSize(t->left) + t->key_num; //左子树的所有结点的key小于key，且t代表的数也小于key
		size_t tmp_size = getNodeSize(t->right); //记录右子树的node_size
		t->right = insert(t->right, key); //递归插入右子树
		if (getNodeSize(t->right) > tmp_size) { //若右子树的node_size变化，即右子树中新建了结点
			t->node_size++;
			t = maintainRight(t); //维护平衡
		}
	}
	return t;
}

void deleteTree(Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	free(t);
}
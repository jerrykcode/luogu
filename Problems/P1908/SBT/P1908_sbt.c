#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t key_num, num_size, node_size;
	struct TNode *left, *right;
} *Tree;

size_t getKeyNum(Tree t) { return t ? t->key_num : 0; }
size_t getNumSize(Tree t) { return t ? t->num_size : 0; }
size_t getNodeSize(Tree t) { return t ? t->node_size : 0; }

Tree newTNode(int key); //产生新结点
Tree leftRotate(Tree t); //左旋
Tree rightRotate(Tree t); //右旋
Tree maintain(Tree t); //维护平衡
Tree maintainLeft(Tree t); //维护平衡--判断是否存在并处理左子树的子树的node_size大于右子树的node_size的情况
Tree maintainRight(Tree t);  //维护平衡--判断是否存在并处理右子树的子树的node_size大于左子树的node_size的情况

Tree insert(Tree t, int key); //插入key
int greater_num_; //global变量，用于计算有多少数大于待插入数
void deleteTree(Tree t); //删除内存

int main() {
	int n, key;
	scanf("%d", &n);
	long long result = 0;
	Tree t = NULL;
	for (int i = 0; i < n; i++) {
		scanf("%d", &key);
		greater_num_ = 0;
		t = insert(t, key);
		result += greater_num_;
	}
	printf("%lld", result);
	deleteTree(t);
	return 0;
}

Tree newTNode(int key) {
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->key = key;
	t->key_num = 1;
	t->num_size = 1;
	t->node_size = 1;
	t->left = NULL;
	t->right = NULL;
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
	//维护平衡--判断是否存在并处理左子树的子树的node_size大于右子树的node_size的情况
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
	//维护平衡--判断是否存在并处理右子树的子树的node_size大于左子树的node_size的情况
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
	t->num_size++; //代表的数量增1
	if (key == t->key) {
		t->key_num++; //重复出现
		greater_num_ += getNumSize(t->right); //右边所有结点代表的数都大于key
	}
	else if (key < t->key) {
	 	//右边所有结点代表的数以及t结点代表的数都大于key
		greater_num_ += getNumSize(t->right) + t->key_num;
		size_t tmp_size = getNodeSize(t->left); //左子树的node_size
		t->left = insert(t->left, key); //递归插入左子树
		if (getNodeSize(t->left) > tmp_size) { //若左子树的node_size变化，即左子树中产生了新结点
			t->node_size++;
			t = maintainLeft(t); //左子树中产生了新结点，需要维护平衡
		}
		//若左子树中没有产生新结点，则树的结构不变，不会破坏平衡
	}
	else {
		size_t tmp_size = getNodeSize(t->right); //右子树的node_size
		t->right = insert(t->right, key); //递归插入右子树
		if (getNodeSize(t->right) > tmp_size) { //若右子树的node_size变化，即右子树中产生了新结点
			t->node_size++;
			t = maintainRight(t); //右子树中产生了新结点，需要维护平衡
		}
		//若右子树中没有产生新结点，则树的结构不变，不会破坏平衡
	}
	return t;
}

void deleteTree(Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	free(t);
}
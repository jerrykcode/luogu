# P1168 中位数
[原题地址](https://www.luogu.org/problem/P1168)

## 题目描述

给出一个长度为N的非负整数序列Ai，对于所有1 <= k <= (N + 1) / 2，输出A1, A3, …, A2k - 1的中位数。即前1,3,5,…个数的中位数。

## 输入格式

第1行为一个正整数N，表示了序列长度。

第2行包含N个非负整数Ai (Ai <= 10^9)。

## 输出格式

共(N + 1)/2行，第i行为A1, A3, …, A2k - 1的中位数。

## 输入输出样例

### 输入 #1
> 7  
> 1 3 5 7 9 11 6

### 输出 #1
> 1  
> 3  
> 5  
> 6

说明/提示

对于20%的数据，N <= 100；

对于40%的数据，N <= 3000；

对于100%的数据，N <= 100000。

## 思路

使用以结点数量作为平衡依据的平衡二叉索搜树 `SBT树`
这个思路和[P2343 宝石管理系统](https://www.luogu.org/problem/P2343)的[做法](https://www.luogu.org/blog/jerrykcode/solution-p2343)相似。

对于每一个整数Ai，将其插入SBT，若i为奇数，则计算在SBT中有`i/2`个数小于它的数，即所有已插入元素的中位数。

定义SBT:
```cpp

	typedef struct TNode {
		int key;
		size_t size;
		struct TNode *left, *right;
		TNode(int key) : key(key), size(1), left(NULL), right(NULL) {}
	} *Tree;

```
key为存储的元素，且二叉索搜树按key排序，size为所有子结点(包括自己)的数量

使用 `int select(Tree tree, int rank);` 计算tree中有`rank`个数小于它的数:
设`tree的左子树的size为left_size`
* 若rank == left_size，则说明正好有rank个数小于tree->key，则返回tree->key即可;
* 若rank < left_size，则递归`select(tree->left, rank)`;
* 若rank > left_size，则递归右子树，且由于左子树的所有结点及tree结点的key都小于等于右子树的任意一个结点的key，所以在右子树中只有`rank - 左子树的结点数量 - 1`个数小于它，即`select(tree->right, rank - left_size - 1)`，

SBT的插入操作就不写了。

##代码

C++

```cpp
#include <cstdio>
#include <cstdlib>

class SBT {
public:
	void insert(int key);
	int select(int rank);
	SBT() : tree_(NULL) {

	}
	~SBT() {
		deleteTree(tree_);
	}
private:
	typedef struct TNode {
		int key;
		size_t size;
		struct TNode *left, *right;
		TNode(int key) : key(key), size(1), left(NULL), right(NULL) {}
	} *Tree;

	size_t getSize(Tree tree) { return tree ? tree->size : 0; }

	Tree leftRotate(Tree tree);
	Tree rightRotate(Tree tree);

	Tree maintain(Tree tree);
	Tree maintainLeft(Tree tree);
	Tree maintainRight(Tree tree);

	Tree insert(Tree tree, int key);
	int select(Tree tree, int rank);

	void deleteTree(Tree tree);
private:
	Tree tree_;
};

void SBT::insert(int key) {
	tree_ = insert(tree_, key);
}

int SBT::select(int rank) {
	return select(tree_, rank);
}

SBT::Tree SBT::leftRotate(Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = getSize(tree->left) + getSize(tree->right) + 1;
	k->size = tree->size + getSize(k->right) + 1;
	return k;
}

SBT::Tree SBT::rightRotate(Tree tree) {
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->size = getSize(tree->left) + getSize(tree->right) + 1;
	k->size = getSize(k->left) + tree->size + 1;
	return k;
}

SBT::Tree SBT::maintain(Tree tree) {
	tree = maintainLeft(tree);
	return maintainRight(tree);
}

SBT::Tree SBT::maintainLeft(Tree tree) {
	if (tree == NULL || tree->left == NULL) return tree;
	if (tree->left->left && tree->left->left->size > getSize(tree->right)) {
		tree = rightRotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	if (tree->left->right && tree->left->right->size > getSize(tree->right)) {
		tree->left = leftRotate(tree->left);
		tree = rightRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

SBT::Tree SBT::maintainRight(Tree tree) {
	if (tree == NULL || tree->right == NULL) return tree;
	if (tree->right->right && tree->right->right->size > getSize(tree->left)) {
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	if (tree->right->left && tree->right->left->size > getSize(tree->left)) {
		tree->right = rightRotate(tree->right);
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

SBT::Tree SBT::insert(Tree tree, int key) {
	if (tree == NULL) {
		return new TNode(key);
	}
	tree->size++;
	if (key < tree->key) {
		tree->left = insert(tree->left, key);
		tree = maintainLeft(tree);
	}
	else {
		tree->right = insert(tree->right, key);
		tree = maintainRight(tree);
	}
	return tree;
}

int SBT::select(Tree tree, int rank) {
	size_t left_size = getSize(tree->left);
	if (rank == left_size) {
		return tree->key;
	}
	else if (rank < left_size) {
		return select(tree->left, rank);
	}
	else {
		return select(tree->right, rank - left_size - 1);
	}
}

void SBT::deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	delete tree;
}
int main() {
	int n, key;
	SBT sbt;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &key);
		sbt.insert(key);
		if (i % 2) {
			printf("%d\n", sbt.select(i / 2));
		}
	}
	return 0;
}
```
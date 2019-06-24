# P3871 [TJOI2010]中位数

[原题地址](https://www.luogu.org/problemnew/show/P3871)

## 题目描述

给定一个由N个元素组成的整数序列，现在有两种操作：

1 add a

在该序列的最后添加一个整数a，组成长度为N + 1的整数序列

2 mid 输出当前序列的中位数

中位数是指将一个序列按照从小到大排序后处在中间位置的数。（若序列长度为偶数，则指处在中间位置的两个数中较小的那个）

例1：1 2 13 14 15 16 中位数为13

例2：1 3 5 7 10 11 17 中位数为7

例3：1 1 1 2 3 中位数为1

## 输入输出格式

### 输入格式：
第一行为初始序列长度N。第二行为N个整数，表示整数序列，数字之间用空格分隔。第三行为操作数M，即要进行M次操作。下面为M行，每行输入格式如题意所述。

### 输出格式：
对于每个mid操作输出中位数的值

## 输入输出样例

### 输入样例#1： 

> 6  
> 1 2 13 14 15 16  
> 5  
> add 5  
> add 3  
> mid  
> add 20  
> mid

### 输出样例#1： 

> 5  
> 13

## 说明

对于30%的数据，1 ≤ N ≤ 10,000，0 ≤ M ≤ 1,000

对于100%的数据，1 ≤ N ≤ 100,000，0 ≤ M ≤ 10,000

序列中整数的绝对值不超过1,000,000,000，序列中的数可能有重复

每个测试点时限1秒

## 思路

SBT树

将最开始输入的n个数字及之后add的数字都插入SBT中。对于mid，设SBT中的数字数量为size，若size为奇数，则有size/2个数小于等于中位数；若size为偶数，
则有size/2 - 1个数小于等于中位数。即mid需要一个函数``` rank(SBT tree, size_t size) ```，用于返回SBT中的一个数，而SBT中有size个数小于等于它。

关于SBT的两个操作：

* 插入

* 查找一个数，有size个数小于等于它：从根结点开始，
	* 若左子树的size正好等于size，则根结点存储的数即所求；
	* 若左子树的size大于size，则递归，此时在左子树上有size个数小于等于它；
	* 若左子树的size小于size，则递归，在右子树上寻找它，但由于右子树的结点已经大于等于左子树及根结点，所以此时在右子树上有(size - 左子树的size - 1)个数小于等于它


## 代码

```cpp
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
```
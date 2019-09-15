# P1428 小鱼比可爱
[原题地址](https://www.luogu.org/problem/P1428)

## 题目描述

人比人，气死人；鱼比鱼，难死鱼。小鱼最近参加了一个“比可爱”比赛，比的是每只鱼的可爱程度。参赛的鱼被从左到右排成一排，头都朝向左边，然后每只鱼会得到一个整数数值，表示这只鱼的可爱程度，很显然整数越大，表示这只鱼越可爱，而且任意两只鱼的可爱程度**可能一样**。由于所有的鱼头都朝向左边，所以每只鱼只能看见在它左边的鱼的可爱程度，它们心里都在计算，在自己的眼力范围内有多少只鱼不如自己可爱呢。请你帮这些可爱但是鱼脑不够用的小鱼们计算一下。

## 输入格式

第一行输入一个整数n，表示鱼的数目。

第二行内输入n个整数，用空格间隔，依次表示从左到右每只小鱼的可爱程度。

## 输出格式

行内输出n个整数，用空格间隔，依次表示每只小鱼眼中有多少只鱼不如自己可爱。

## 输入输出样例

**输入 #1**

> 6  
> 4 3 0 5 1 2

**输出 #1**

> 0 0 0 3 1 2


## 说明/提示

n<=100

## 思路

将每个整数插入序列，同时计算已插入序列中有多少整数小于该整数。

#### 简单的嵌套循环即可解决，但这里是来小题大做的

### 树状数组
假设有数组a，size足够大，初始化为0，对于插入的每个整数val，a[val]++，那么对于a的树状数组c，每次插入整数val时，先通过树状数组c计算原数组a[0]~a[val - 1]之和，即在插入val之前序列中小于val的整数个数，然后单点更新树状数组即可。
但由于插入的整数没有限定范围，那么树状数组的尺寸需要非常大。所以这里需要离散化数据，即输入数据的数量远小于数据的范围，可以给数据重新赋值而结果不变，如插入`1, 100, 1000, 10000000`和插入`1, 2, 3, 4`对本问题没有影响。

```c
struct node {
	int val;
	int input_rank;
};

```
使用此结构体的数组，记录每个输入的整数及其输入的次序。对数组排序，val小的排在前面，val相同则input_rank大的，即`后输入`的排在前面。然后对排序后的每个整数"赋新的值"，排在最前面的赋值为1，之后为2...

之后再次遍历输入的整数，按它们的"新值"通过树状数组进行计算。

(注意排序时val相同则后输入的排前面，这和求逆序对时有所不同，因为求逆序对是要计算出`小于等于`val的整数的数量从而求出大于val的整数的数量。而这里是要计算`小于`val的整数的数量。)



### SBT树

另外一种小题大做的做法是使用SBT树，一种通过结点数量维护平衡的平衡树。

定义树结点:
```c
typedef struct TNode {
	int key;
	size_t size;
	struct TNode *left, *right;
} *SBT;

```
其中，key为插入的整数，二叉搜索树也依据key排序，size为所有子结点(包括自己)的数量。

对于每个整数，插入SBT平衡树即可，若待插入整数key >= tree->key，则递归插入右子树，且tree->left的所有结点小于key，如果key > tree->key，那么小于key的整数数量还要加1。

即插入key的过程中即可求出树中小于key的整数数量。

## 代码

### 树状数组

```c
#include "stdio.h"
#include "stdlib.h"

int * c, n;

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (; x <= n; x += lowbit(x)) {
		c[x] += k;
	}
}

int sum(int x) {
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += c[x];
	}
	return result;
}

struct node {
	int val;
	int input_rank;
};

int compare(const void * a, const void * b) {
	return ((struct node *)a)->val != ((struct node *)b)->val ? ((struct node *)a)->val - ((struct node *)b)->val : 
		((struct node *)b)->input_rank - ((struct node *)a)->input_rank;
}

int main() {
	scanf("%d", &n);
	struct node * node_array = (struct node *)malloc(n * sizeof(struct node));
	int i = 0, val;
	for (; i < n; i++) {
		scanf("%d", &val);
		node_array[i].val = val;
		node_array[i].input_rank = i;
	}
	qsort(node_array, n, sizeof(struct node), compare);
	int * rank = (int *)malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		rank[node_array[i].input_rank] = i + 1;
	}
	free(node_array);	
	c = (int *)malloc((n + 1) * sizeof(int));
	for (i = 1; i < n + 1; i++) c[i] = 0;
	for (i = 0; i < n; i++) {	
		if (i != 0) putchar(' ');
		printf("%d", sum(rank[i] - 1));
		add(rank[i], 1);
	}
	free(rank);
	free(c);
	return 0;
}
```

### SBT树

```c
#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t size;
	struct TNode *left, *right;
} *SBT;

#define SIZE(tree) ((tree) ? (tree->size) : 0)

SBT newTNode(int key);
SBT leftRotate(SBT tree);
SBT rightRotate(SBT tree);
SBT maintain(SBT tree);
SBT maintainLeft(SBT tree);
SBT maintainRight(SBT tree);
SBT insert(SBT tree, int key);
void deleteTree(SBT tree);

SBT tree = NULL;
size_t smaller_num;

int main() {
	int n, key;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &key);
		smaller_num = 0;
		tree = insert(tree, key);
		if (i != 0) putchar(' ');
		printf("%d", smaller_num);
	}
	deleteTree(tree);
	return 0;
}

SBT newTNode(int key) {
	SBT tree = (SBT)malloc(sizeof(struct TNode));
	tree->key = key;
	tree->size = 1;
	tree->left = tree->right = NULL;
}

SBT leftRotate(SBT tree) {
	if (!tree || !tree->right) return tree;
	SBT k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = tree->size + SIZE(k->right) + 1;
	return k;
}

SBT rightRotate(SBT tree) {
	if (!tree || !tree->left) return tree;
	SBT k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = SIZE(k->left) + tree->size + 1;
	return k;
}

SBT maintain(SBT tree) {
	tree = maintainLeft(tree);
	return maintainRight(tree);
}

SBT maintainLeft(SBT tree) {
	if (!tree) return tree;
	if (!tree->left) return tree;
	if (tree->left->left && tree->left->left->size > SIZE(tree->right)) {
		tree = rightRotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	if (tree->left->right && tree->left->right->size > SIZE(tree->right)) {
		tree->left = leftRotate(tree->left);
		tree = rightRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

SBT maintainRight(SBT tree) {
	if (!tree) return tree;
	if (!tree->right) return tree;
	if (tree->right->right && tree->right->right->size > SIZE(tree->left)) {
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	if (tree->right->left && tree->right->left->size > SIZE(tree->left)) {
		tree->right = rightRotate(tree->right);
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

SBT insert(SBT tree, int key) {
	if (tree == NULL) {
		return newTNode(key);
	}
	tree->size++;
	if (key < tree->key) {
		tree->left = insert(tree->left, key);
		tree = maintainLeft(tree);
	}
	else { //key >= tree->key
		smaller_num += SIZE(tree->left);
		if (key > tree->key) smaller_num++;
		tree->right = insert(tree->right, key);
		tree = maintainRight(tree);
	}
	return tree;
}

void deleteTree(SBT tree) {
	if (!tree) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
}
```

> Written with [StackEdit](https://stackedit.ioaa/).
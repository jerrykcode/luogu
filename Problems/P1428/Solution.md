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

#### 之前SBT的做法虽然AC，但其实是有bug的！
>另外一种小题大做的做法是使用SBT树，一种通过结点数量维护平衡的平衡树。
>
>定义树结点:
>```c
>typedef struct TNode {
>	int key;
>	size_t size;
>	struct TNode *left, *right;
>} *SBT;
>
>```
>其中，key为插入的整数，二叉搜索树也依据key排序，size为所有子结点(包括自己)的数量。
>
>对于每个整数，插入SBT平衡树即可，若待插入整数key >= tree->key，则递归插入右子树，且tree->left的所有结点小于key，如果key > tree->key，那么小于key的整数数量还要加1。
>
>即插入key的过程中即可求出树中小于key的整数数量。

在插入相同数时，会有bug，如这一组输入(input1)
10
1 1 1 1 1 1 1 1 1 1
正确应输出全0，但之前的SBT代码输出 0 0 0 1 1 2 3 4 4 5

#### BUG
二叉索搜树的性质是左边<根<右边，但当key == t->key时，key应该递归插入左子树还是右子树呢？之前的代码是插入了右子树，那么考虑插入10个1的情况，树本来会形成一根链表，但是维护平衡后，树的左子树的所有结点与右子树的所有结点以及树根的key全部是1，此时树就已经不是二叉搜索树了！！！

#### 正解
对于重复插入的数，一种正确的做法是`将相同的数用同一个结点记录`。  
那么显然树结点中要增加一个`key_num`属性用来记录key出现了多少次，插入相同的key时，key_num++即可。  
`size`属性变为`node_size`，表示树有多少结点，用来维护平衡，  
注意此时一个结点可能代表多个数，所以还要用`num_size`属性记录树中插入了多少个数。插入过程中计算有多少个数小于待插入数，就是基于`num_size`属性的。
```c
typedef struct TNode {
	int key;
	size_t key_num;
	size_t num_size;
	size_t node_size;
	struct TNode *left, *right;
} *Tree;
```



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

以下是改正后的代码SBT/P1428.c，之前的错误代码在SBT/P1428_bug.c中

```c
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
```

> Written with [StackEdit](https://stackedit.ioaa/).

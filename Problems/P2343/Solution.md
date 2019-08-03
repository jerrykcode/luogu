# P2343 宝石管理系统
[原题地址](https://www.luogu.org/problem/P2343)

## 题目描述
GY君购买了一批宝石放进了仓库。有一天GY君心血来潮，想要清点他的宝石，于是把m个宝石都取出来放进了宝石管理系统。每个宝石i都有一个珍贵值vi，
他希望你能编写程序查找到从大到小第n珍贵的宝石。但是现在问题来了，他非常不小心的留了一些宝石在仓库里面，有可能要往现有的系统中添加宝石。
这些宝石的个数比较少。他表示非常抱歉，但是还是希望你的系统能起作用。

## 输入格式

第一行一个整数m，q，表示已经取出来的宝石个数以及接下来的查询或插入操作个数。

第二行m个整数，表示这m个宝石的珍贵值。

以下q行，每行两个整数c，n,

若c=1（即询问），则输出当前第n珍贵的宝石，

若c=2（即插入），则往系统中插入珍贵值为n的宝石。

## 输出格式

对于每个c=1（询问）,输出当前第n珍贵的宝石的珍贵值vi。

## 输入输出样例

### 输入 #1 
> 5 3  
> 1 3 2 5 6  
> 1 3  
> 2 4  
> 1 6  
### 输出 #1
> 3  
> 1  
## 说明/提示
对于50%的数据，没有c=2的情况；

对于100%的数据，m<=100000，c=2的情况不超过10000，q<=30000

## 思路

使用依据结点数量平衡的SBT树。
* 对于插入操作，将新结点插入SBT即可
* 对于查询操作，排名为n即有n - 1个元素大于它，用函数``` int rank(Tree tree, int n) ```查询tree中有n个结点比它大的结点 ，
	那么```rank(n - 1)```就是查询的答案。
	rank函数的实现：由于SBT树的每个结点记录了以该结点为根的树的结点数量(size)，从根结点tree开始递归：
	设`tree的右子树的size`为`rightSize`
	* 若n == rightSize，则正好有n个结点比tree大，那么tree就是查询的结点。
	* 若n < rightSize，则递归```rank(tree->right, n)```
	* 若n > rightSize，则递归```rank(tree->left, n - rightSize - 1)```

## 代码

### C

```c
#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t size;
	struct TNode * left;
	struct TNode * right;
} *Tree;

#define SIZE(tree) (tree ? tree->size : 0)

Tree newTNode(int key) {
	Tree tree = (Tree)malloc(sizeof(struct TNode));
	tree->key = key;
	tree->size = 1;
	tree->left = tree->right = NULL;
	return tree;
}

Tree leftRotate(Tree tree);
Tree rightRotate(Tree tree);

Tree maintain(Tree tree);
Tree maintainLeft(Tree tree);
Tree maintainRight(Tree tree);

Tree insert(Tree tree, int key);
int rank(Tree tree, int n);

void deleteTree(Tree tree);

Tree leftRotate(Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = tree->size + SIZE(k->right) + 1;
	return k;
}

Tree rightRotate(Tree tree) {
	Tree k = tree->left;
	tree->left = k -> right;
	k->right = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = SIZE(k->left) + tree->size + 1;
	return k;
}

Tree maintain(Tree tree) {
	tree = maintainLeft(tree);
	tree = maintainRight(tree);
	return tree;
}

Tree maintainLeft(Tree tree) {
	if (tree == NULL) return tree;
	if (tree->left == NULL) return tree;
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

Tree maintainRight(Tree tree) {
	if (tree == NULL) return tree;
	if (tree->right == NULL) return tree;
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

Tree insert(Tree tree, int key) {
	if (tree == NULL) {
		return newTNode(key);
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

int rank(Tree tree, int n) {
	int right_size = SIZE(tree->right);
	if (n == right_size) {
		return tree->key;
	}
	else if (n < right_size) {
		return rank(tree->right, n);
	}
	else {
		return rank(tree->left, n - right_size - 1);
	}
}

void deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
}

int main() {
	Tree tree = NULL;
	int m, q;
	scanf("%d %d", &m, &q);
	int c, n;
	for (int i = 0; i < m; i++) {
		scanf("%d", &n);
		tree = insert(tree, n);
	}	
	for (int i = 0; i < q; i++) {
		scanf("%d %d", &c, &n);
		if (c == 1) {
			printf("%d\n", rank(tree, n - 1));
		}
		else {
			tree = insert(tree, n);			
		}
	}
	deleteTree(tree);
	return 0;
}
```
# P2073 送花
[原题地址](https://www.luogu.org/problemnew/show/P2073)


## 题目背景

小明准备给小红送一束花，以表达他对小红的爱意。他在花店看中了一些花，准备用它们包成花束。

## 题目描述

这些花都很漂亮，每朵花有一个美丽值W，价格为C。

小明一开始有一个空的花束，他不断地向里面添加花。他有以下几种操作：

操作 含义

1 W C 添加一朵美丽值为W，价格为C的花。

3 小明觉得当前花束中最便宜的一朵花太廉价，不适合送给小红，所以删除最便宜的一朵花。

2 小明觉得当前花束中最贵的一朵花太贵，他心疼自己的钱，所以删除最贵的一朵花。

-1 完成添加与删除，开始包装花束

若删除操作时没有花，则跳过删除操作。

如果加入的花朵价格已经与花束中已有花朵价格重复，则这一朵花不能加入花束。

请你帮小明写一个程序，计算出开始包装花束时，花束中所有花的美丽值的总和，以及小明需要为花束付出的总价格。

## 输入输出格式

### 输入格式：

若干行，每行一个操作，以-1结束。

### 输出格式：

一行，两个空格隔开的正整数表示开始包装花束时，花束中所有花的美丽值的总和。以及小明需要为花束付出的总价格。

## 输入输出样例

### 输入样例#1： 

> 1 1 1  
> 1 2 5  
> 2  
> 1 3 3  
> 3  
> 1 5 2  
> -1


### 输出样例#1： 

> 8 5


## 说明

对于20%数据，操作数<=100，1<=W,C<=1000。

对于全部数据，操作数<=100000,1<=W,C<=1000000。

## 思路

平衡树即可。

这里使用通过`左右子树高度`维护平衡的`AVL树`。

注意删除结点时，只需要删除最大或最小结点。

## 代码

```c
#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

typedef struct AVLTreeNode {
	int w;
	int c; //
	struct AVLTreeNode * left;
	struct AVLTreeNode * right;
	int height;
} *AVLTree;

#define HEIGHT(tree) (tree ? tree->height : 0)

//"构造函数"
AVLTree newAVLTreeNode(int w, int c) {
	AVLTree tree = (AVLTree)malloc(sizeof(struct AVLTreeNode));
	tree->w = w;
	tree->c = c;
	tree->left = NULL;
	tree->right = NULL;
	tree->height = 1;
	return tree;
}

//左旋
AVLTree leftRotate(AVLTree tree) {
	AVLTree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->height = max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
	k->height = max(HEIGHT(tree), HEIGHT(k->right)) + 1;
	return k;
}

//右旋
AVLTree rightRotate(AVLTree tree) {
	AVLTree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->height = max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
	k->height = max(HEIGHT(k->left), HEIGHT(tree)) + 1;
	return k;
}

AVLTree insert(AVLTree tree, int w, int c) {
	if (tree == NULL) {
		tree = newAVLTreeNode(w, c); //返回新结点
		return tree;
	}
	if (c == tree->c) return tree; //价值为c的结点已存在，直接返回tree
	else if (c < tree->c) { //插入左子树
		tree->left = insert(tree->left, w, c); //递归左子树
		tree->height = max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
		if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2) { 
			if (c > tree->left->c) {
				//插入左子树的右子树
				tree->left = leftRotate(tree->left);
			}
			tree = rightRotate(tree);
		}
	}
	else { //与插入左子树对称
		tree->right = insert(tree->right, w, c);
		tree->height = max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
		if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2) {
			if (c < tree->right->c) {
				tree->right = rightRotate(tree->right);
			}
			tree = leftRotate(tree);
		}
	}
	return tree;
}

AVLTree deleteMin(AVLTree tree) {
	if (tree == NULL) return NULL;
	if (tree->left == NULL) { //没有左子树了, tree是最小的
		AVLTree tmp = tree;
		tree = tree->right; //右子树代替tree，若没有右子树，tree更新为NULL
		free(tmp);
		return tree;
	}
	tree->left = deleteMin(tree->left);
	tree->height = max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
	if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2) {
		if (HEIGHT(tree->right->left) > HEIGHT(tree->right->right)) {
			//相当于插入函数中，插入右子树的左子树的情况
			tree->right = rightRotate(tree->right);
		}
		tree = leftRotate(tree);
	}
	return tree;
}

AVLTree deleteMax(AVLTree tree) {
	//与deleteMin对称
	if (tree == NULL) return NULL;
	if (tree->right == NULL) {				
		AVLTree tmp = tree;
		tree = tree->left;
		free(tmp);
		return tree;
	}
	tree->right = deleteMax(tree->right);
	tree->height = max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;
	if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2) {
		if (HEIGHT(tree->left->right) > HEIGHT(tree->left->left)) {
			//相当于插入左子树的右子树的情况
			tree->left = leftRotate(tree->left);
		}
		tree = rightRotate(tree);
	}
	return tree;
}

int total_w = 0, total_c = 0;
void dfs(AVLTree tree) {
	if (tree == NULL) return;
	total_w += tree->w;
	total_c += tree->c;
	dfs(tree->left);
	dfs(tree->right);
}

void deleteTree(AVLTree tree) {
	if (tree == NULL) return;
	deleteTree(tree->left);
	deleteTree(tree->right);
	free(tree);
}

int main() {
	AVLTree tree = NULL;
	int cmd, w, c;
	while (1) {
		scanf("%d", &cmd);
		if (cmd == -1) {		
			dfs(tree);	
			printf("%d %d\n", total_w, total_c);
			break;
		}
		else if (cmd == 1) {
			scanf("%d %d", &w, &c);
			tree = insert(tree, w, c);
		}
		else if (cmd == 2) {
			tree = deleteMax(tree);
		}
		else if (cmd == 3) {
			tree = deleteMin(tree);
		}
	}
	deleteTree(tree);
	return 0;
}
```
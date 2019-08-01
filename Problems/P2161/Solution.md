# P2161 [SHOI2009]会场预约
[原题地址](https://www.luogu.org/problem/P2161)

## 题目描述

PP大厦有一间空的礼堂，可以为企业或者单位提供会议场地。这些会议中的大多数都需要连续几天的时间（个别的可能只需要一天），不过场地只有一个，所以不同的会议的时间申请
不能够冲突。也就是说，前一个会议的结束日期必须在后一个会议的开始日期之前。所以，如果要接受一个新的场地预约申请，就必须拒绝掉与这个申请相冲突的预约。 一般来说，如
果PP大厦方面事先已经接受了一个会场预约，例如从10日到15日，就不会在接受与之相冲突的预约，例如从12日到17日。不过，有时出于经济利益，PP大厦方面有时会为了接受一个
新的会场预约，而拒绝掉一个甚至几个之前预订的预约。 于是，礼堂管理员QQ的笔记本上笔记本上经常记录着这样的信息： 本题中为方便起见，所有的日期都用一个整数表示。例如，
如果一个为期10天的会议从"90日"开始到"99日"，那么下一个会议最早只能在"100日"开始。 最近，这个业务的工作量与日俱增，礼堂的管理员QQ希望参加SHTSC的你替他设计一套
计算机系统，方便他的工作。这个系统应当能执行下面两个操作： A操作：有一个新的预约是从"start日"到"end日"，并且拒绝掉所有与它相冲突的预约。执行这个操作的时候，你的系
统应当返回为了这个新预约而拒绝掉的预约个数，以方便QQ与自己的记录相校对。 B操作：请你的系统返回当前的仍然有效的预约的总数。

## 输入格式

输入文件的第一行是一个整数n，表示你的系统将接受的操作总数。 
接下去n行每行表示一个操作。每一行的格式为下面两者之一： 

* "A start end"表示一个A操作； 
* "B"表示一个B操作。

## 输出格式

输出文件有n行，每行一次对应一个输入。表示你的系统对于该操作的返回值。

## 输入输出样例

### 输入 #1

> 6  
> A 10 15  
> A 17 19  
> A 12 17  
> A 90 99  
> A 11 12  
> B  

## 输出 #1

> 0  
> 0  
> 2  
> 0  
> 1  
> 2  

### 说明/提示

N< = 200000

1< = Start End < = 100000

## 思路

### 二叉索搜树

树结点存储start及end，表示一个预约
```c
typedef struct TNode {
	int start, end;
	struct TNode * left;
	struct TNode * right;
} * Tree;
```

对每个树结点：

* start < end
* 左子树中所有结点的start和end都小于start
* 右子树中所有结点的start和end都大于end

树中所有的结点的(start, end)都不存在重叠部分，即不存在冲突的预约

### A操作

声明函数:
```c
Tree insert(Tree tree, int start, int end);
```
向tree中插入一个结点(start, end)，若与已有结点存在重叠，则删除已有结点。返回插入后的树。

对二叉索搜树插入稍微更改即可：

* 若tree为NULL，新建结点并返回
* 若end < tree->start，递归插入左子树
* 若start > tree->end, 递归插入右子树
* 否则待插入的(start, end)与(tree->start, tree->end)有重叠，`(tree->start, tree->end)表示的预约被拒绝`。注意此时`tree的子树中可能也存在与(start, end)重叠的结点`，若：
	* `start < tree->start`。左子树中所有结点的start和end均小于tree->start，但若start < tree->start，
	则tree的左子树中可能存在结点subtree，使subtree->end > start，而subtree->end < tree->start < end，即`start < subtree->end < end`，即subtree与(start, end)重叠，
	调用```tree->left = leftOverlap(tree->left, start);```消除tree的左子树中的重叠，`leftOverlap`函数见下文。
	*  `end > tree->end`。右子树中所有结点的start和end均大于tree->end，但若end > tree->end，
	则tree的右子树中可能存在结点subtree，使subtree->start < end，而subtree->start > tree->end > start，即`start < subtree->start < end`，即subtree与(start, end)重叠，
	调用```tree->right = rightOverlap(tree->right, end);```消除tree的右子树中的重叠，`rightOverlap`函数见下文。
  
  以上两种情况可能同时发生。`最后使用(start, end)替换(tree->start, tree->end)，即插入了新预约同时拒绝所有冲突的预约`

* leftOverlap函数
```c
Tree leftOverlap(Tree tree, int start);
```
对tree中所有结点subtree，若subtree->end > start，则删除此结点，即拒绝了冲突的预约，返回更新后的树

* 若tree为NULL，不存在冲突的预约，返回 NULL
* 若tree->end < start，则tree结点存储的预约不冲突，且tree的左子树的所有结点也一定不冲突。递归右子树。
* 若tree->end >= start，则tree结点存储的预约冲突，且tree的右子树的所有结点也一定冲突，删除右子树的所有结点，用left存储tree的左子树，然后删除tree结点本身。
   用left取代tree，此时还要递归left，因为left中可能还存在冲突的结点。

* rightOverlap函数

与leftOverlap对称。

#### 程序中使用全局变量tree_size表示树的结点数，removed_node_num表示因插入而删除的冲突结点数。
#### main中将tree_size初始化为0，每次A操作中吧removed_node_num初始化为0，然后调用insert函数。
#### 新建结点时 tree_size++；删除结点时tree_size--，removed_node_num++；替换结点时removed_node_num++(替换后树的结点数不变，但被替换的结点要被算作拒绝的结点)
#### A操作最后输出removed_node_num。

### B操作  输出tree_size即可

## 代码 

### C

```c
#include "stdio.h"
#include "stdlib.h"

int tree_size;
int removed_node_num;

typedef struct TNode {
	int start, end;
	struct TNode * left;
	struct TNode * right;
} * Tree;

Tree newTNode(int start, int end) {
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->start = start;
	t->end = end;
	t->left = t->right = NULL;
	return t;
}

Tree insert(Tree tree, int start, int end);
Tree leftOverlap(Tree tree, int start);
Tree rightOverlap(Tree tree, int end);
void deleteTree(Tree tree);

Tree insert(Tree tree, int start, int end) {
	if (tree == NULL) {
		tree_size++;	
		return newTNode(start, end);
	}
	if (end < tree->start) {
		tree->left = insert(tree->left, start, end);
	}
	else if (start > tree->end) {
		tree->right = insert(tree->right, start, end);
	}
	else {
		if (start < tree->start) {
			tree->left = leftOverlap(tree->left, start);
		}
		if (end > tree->end) {
			tree->right = rightOverlap(tree->right, end);
		}
		tree->start = start;
		tree->end = end;
		removed_node_num++;
	}
	return tree;
}

Tree leftOverlap(Tree tree, int start) {
	if (tree == NULL) return NULL;
	if (tree->end < start) {
		tree->right = leftOverlap(tree->right, start);
		return tree;
	}
	else {
		deleteTree(tree->right);
		tree->right = NULL;
		Tree left = tree->left;
		free(tree);
		removed_node_num++;
		tree_size--;
		return leftOverlap(left, start);
	}
}

Tree rightOverlap(Tree tree, int end) {
	if (tree == NULL) return NULL;
	if (tree->start > end) {
		tree->left = rightOverlap(tree->left, end);
		return tree;
	}
	else {
		deleteTree(tree->left);
		tree->left = NULL;
		Tree right = tree->right;
		free(tree);
		removed_node_num++;
		tree_size--;
		return rightOverlap(right, end);
	}
}

void deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
	removed_node_num++;
	tree_size--;
}

int main() {
	Tree tree = NULL;
	tree_size = 0;
	int n ,start, end;
	scanf("%d", &n);
	char cmd;	
	for (int i = 0; i < n; i++) {
		scanf(" %c", &cmd);
		if (cmd == 'A') {
			scanf("%d %d", &start, &end);
			removed_node_num = 0;
			tree = insert(tree, start, end);
			printf("%d\n", removed_node_num);
		}
		else {
			printf("%d\n", tree_size);
		}
	}
	deleteTree(tree);
	return 0;
}
```
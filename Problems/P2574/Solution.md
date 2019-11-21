# P2574 XOR的艺术
[原题地址](https://www.luogu.org/problem/P2574)

## 题目描述

AKN觉得第一题太水了，不屑于写第一题，所以他又玩起了新的游戏。在游戏中，他发现，这个游戏的伤害计算有一个规律，规律如下

1、 拥有一个伤害串为长度为n的01串。

2、 给定一个范围[l,r]，伤害为伤害串的这个范围内中1的个数

3、 会被随机修改伤害串中的数值，修改的方法是把[l,r]中的所有数xor上1

AKN想知道一些时刻的伤害，请你帮助他求出这个伤害

## 输入格式

第一行两个数n,m,表示长度为n的01串，有m个时刻

第二行一个长度为n的01串，为初始伤害串

第三行开始m行，每行三个数p,l,r

若p为0，则表示当前时刻改变[l,r]的伤害串，改变规则如上

若p为1，则表示当前时刻AKN想知道[l,r]的伤害

## 输出格式

对于每次询问伤害，输出一个数值伤害，每次询问输出一行

## 输入输出样例
### 输入 #1
> 10 6  
1011101001  
0 2 4  
1 1 5  
0 3 7  
1 1 10  
0 1 4  
1 2 6


### 输出 #1
> 3  
6  
1

### 说明/提示

样例解释：

1011101001

1100101001

询问[1,5]输出3

1111010001

询问[1,10]输出6

0000010001

询问[2,6]输出1

数据范围：

10%数据2≤n,m≤10

另有30%数据2≤n,m≤2000

100%数据2≤n,m≤2*10^5

By:worcher

## 思路
用线段树维护二进制串，  
一个结点记录一段二进制串之和。  
对于求伤害操作，即求一段中1的个数，那么用线段树求和即可。  
对于更新操作，与1 xor 实际上是把一段二进制串的0变为1，1变为0，  
那么`更新后1的个数之和就是更新前0的个数之和`  
即把这一段的和更新为`这一段的长度减去原来的和即可`，  
懒标记为一个bool，true表示它的子树需要xor，false表示不需要。  
初始时所有结点的懒标记均为false，  
更新时，结点xor之后，懒标记要取反。也就是若更新前懒标记为false，更新后懒标记为true，  
若更新前懒标记为true，则表示更新前子树就需要一次xor，那么更新后，子树需要再次xor，两次xor抵消，所以懒标记更新为false。  
在懒标记向下传递时，子树的懒标记也是取反。具体见代码。  

## 代码

```c
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

typedef struct TNode {
	int start, end;
	int sum;
	bool lazy_tag;
	struct TNode *left, *right;
} *SegmentTree;

SegmentTree build(int start, int end, char * binary_str);
void update(int start, int end, SegmentTree st);
int sum(int start, int end, SegmentTree st);
void tagDown(SegmentTree st);
void xor(SegmentTree st);
int len(SegmentTree st);
bool contain(int start, int end, SegmentTree st);
bool cross(int start, int end, SegmentTree st);
void deleteTree(SegmentTree st);

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	char * binary_str = (char *)malloc(n * sizeof(char)), ch;
	for (int i = 0; i < n; i++) {
		do {
			ch = getchar();
		} while (ch != '0' && ch != '1');
		binary_str[i] = ch;
	}
	SegmentTree st = build(0, n - 1, binary_str);
	free(binary_str);
	int p, l, r;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &p, &l, &r);
		l--; r--;
		if (p) {
			printf("%d\n", sum(l, r, st));
		}
		else {
			update(l, r, st);
		}
	}
	deleteTree(st);
	return 0;
}

SegmentTree build(int start, int end, char * binary_str) {
	if (start > end) return NULL;
	SegmentTree st = (SegmentTree)malloc(sizeof(struct TNode));
	st->start = start;
	st->end = end;
	st->lazy_tag = false;
	if (start == end) {
		st->left = st->right = NULL;
		st->sum = (int)(binary_str[start] - '0');
	}
	else {
		int mid = (start + end) >> 1;
		st->left = build(start, mid, binary_str);
		st->right = build(mid + 1, end, binary_str);
		st->sum = st->left->sum + st->right->sum;
	}
	return st;
}

void update(int start, int end, SegmentTree st) {
	if (contain(start, end, st)) {
		xor (st);
		st->lazy_tag = !st->lazy_tag;
		return;
	}
	tagDown(st);
	if (cross(start, end, st->left)) {
		update(start, end, st->left);
	}
	if (cross(start, end, st->right)) {
		update(start, end, st->right);
	}
	st->sum = st->left->sum + st->right->sum;
}

int sum(int start, int end, SegmentTree st) {
	if (contain(start, end, st)) {
		return st->sum;
	}
	tagDown(st);
	int result = 0;
	if (cross(start, end, st->left)) {
		result += sum(start, end, st->left);
	}
	if (cross(start, end, st->right)) {
		result += sum(start, end, st->right);
	}
	return result;
}

void tagDown(SegmentTree st) {
	if (st->lazy_tag) {
		xor (st->left);
		st->left->lazy_tag = !st->left->lazy_tag;
		xor (st->right);
		st->right->lazy_tag = !st->right->lazy_tag;
		st->lazy_tag = false;
	}
}

void xor(SegmentTree st) {
	st->sum = len(st) - st->sum;
}

int len(SegmentTree st) {
	return st->end - st->start + 1;
}

bool contain(int start, int end, SegmentTree st) {
	return start <= st->start && st->end <= end;
}

bool cross(int start, int end, SegmentTree st) {
	return start <= st->end && st->start <= end;
}

void deleteTree(SegmentTree st) {
	if (st == NULL) return;
	deleteTree(st->left);
	deleteTree(st->right);
	free(st);
}
```
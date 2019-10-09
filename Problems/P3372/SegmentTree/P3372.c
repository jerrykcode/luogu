#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#ifndef bool
#define bool	_Bool
#define false	0
#define true	1
#endif

typedef long long LL;

typedef struct TNode {
	int start, end;
	LL sum;
	LL tag;
	struct TNode * left;
	struct TNode * right;
}*Tree;

Tree newTNode(int start, int end) {
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->start = start;
	t->end = end;
	t->left = t->right = NULL;
	//sum 
	return t;
}

Tree buildTree(LL * arr, int start, int end) {
	Tree t = newTNode(start, end);
	if (start == end) {
		t->sum = arr[start];
	}
	else {
		int mid = (start + end) >> 1; //(start + end)/2
		t->left = buildTree(arr, start, mid);
		t->right = buildTree(arr, mid + 1, end);
		t->sum = t->left->sum + t->right->sum;
	}
	t->tag = 0;
	return t;
}

int len(Tree t) {
	return t->end - t->start + 1;
}

void tagDown(Tree t) {
	if (!t->tag) return;
	if (t->left) {
		t->left->sum += (t->tag) * (len(t->left));
		t->left->tag += t->tag;
	}
	if (t->right) {
		t->right->sum += (t->tag) * (len(t->right));
		t->right->tag += t->tag;
	}
	t->tag = 0;
}

bool containTree(Tree t, int start, int end) {
	return start <= t->start &&  t->end <=end;
}

bool crossTree(Tree t, int start, int end) {
	return end >= t->start && start <= t->end;
}

LL query(Tree t, int start, int end) {
	if (containTree(t, start, end)) {
		return t->sum;
	}
	tagDown(t);
	LL result = 0;
	if (crossTree(t->left, start, end)) {
		result += query(t->left, start, end);
	}
	if (crossTree(t->right, start, end)) {
		result += query(t->right, start, end);
	}
	return result;
}

void update(Tree t, int start, int end, LL k) {
	if (containTree(t, start, end)) {
		t->sum += k * len(t);
		t->tag += k;
		return;
	}
	tagDown(t);
	if (crossTree(t->left, start, end)) {
		update(t->left, start, end, k);
	}
	if (crossTree(t->right, start, end)) {
		update(t->right, start, end, k);
	}
	t->sum = t->left->sum + t->right->sum;
}

void deleteTree(Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	free(t);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	LL * arr = (LL *)malloc(n * sizeof(LL));
	for (int i = 0; i < n; i++) {
		scanf("%lld", arr + i);
	}
	Tree t = buildTree(arr, 0, n - 1);
	free(arr);
	int cmd, x, y;
	LL k;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &cmd, &x, &y);
		x--; y--;
		if (cmd == 1) {
			scanf("%lld", &k);
			update(t, x, y, k);
		}
		else {
			printf("%lld\n", query(t, x, y));
		}
	}
	deleteTree(t);
	return 0;
}

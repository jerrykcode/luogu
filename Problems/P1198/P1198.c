#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef struct TNode {
	int start, end;
	int max_;
	struct TNode *left, *right;
} *SegmentTree;

SegmentTree buildTree(int start, int end);
void update(SegmentTree tree, int index, int value);
int query(SegmentTree tree, int start, int end);
void deleteTree(SegmentTree tree);

int main() {
	int index = 0, t = 0;
	int m, d;
	scanf("%d %d", &m, &d);
	SegmentTree tree = buildTree(0, m - 1);
	char cmd;
	for (int i = 0; i < m; i++) {
		scanf(" %c", &cmd);
		if (cmd == 'A') {
			int n;
			scanf("%d", &n);
			update(tree, index++, (n + t) % d);
		}
		else {
			int l;
			scanf("%d", &l);
			printf("%d\n", (t = query(tree, index - l, index - 1)));
		}
	}
	deleteTree(tree);
	return 0;
}

SegmentTree buildTree(int start, int end) {
	if (start > end) return NULL;
	SegmentTree tree = (SegmentTree)malloc(sizeof(struct TNode));
	tree->start = start;
	tree->end = end;
	tree->max_ = 0;
	if (start == end) {
		tree->left = tree->right = NULL;
	}
	else {
		int mid = (start + end) / 2;
		tree->left = buildTree(start, mid);
		tree->right = buildTree(mid + 1, end);
	}
	return tree;
}

void update(SegmentTree tree, int index, int value) {
	if (tree == NULL) return;
	if (tree->start == tree->end) {
		tree->max_ = value;
		return;
	}
	if (index <= tree->left->end) {
		update(tree->left, index, value);
	}
	else {
		update(tree->right, index, value);
	}
	if (value > tree->max_) tree->max_ = value;
}

int query(SegmentTree tree, int start, int end) {
	if (start <= tree->start && tree->end <= end) {
		return tree->max_;
	}
	int result = 0x80000000;
	if (start <= tree->left->end) {
		result = query(tree->left, start, end);
	}
	if (end >= tree->right->start) {
		result = max(result, query(tree->right, start, end));
	}
	return result;
}

void deleteTree(SegmentTree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
}
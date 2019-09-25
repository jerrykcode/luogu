#include "stdio.h"
#include "stdlib.h"

#define MAX_H 1000000
#define MIN_H 1

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef struct TNode {
	int start;
	int end;
	int max_, min_;
	struct TNode * left;
	struct TNode * right;
} * SegmentTree;

SegmentTree buildTree(int * arr, int start, int end);
void query(SegmentTree tree, int start, int end, int * p_max, int * p_min);
void deleteTree(SegmentTree tree);

int main() {
	int n, q, a, b, maxh, minh;
	scanf("%d %d", &n, &q);
	int * arr = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		scanf("%d", arr + i);
	}
	SegmentTree tree = buildTree(arr, 0, n - 1);
	for (int i = 0; i < q; i++) {
		scanf("%d %d", &a, &b);
		a--; b--;		
		query(tree, a, b, &maxh, &minh);
		printf("%d\n", maxh - minh);
	}
	deleteTree(tree);
	return 0;
}

SegmentTree buildTree(int * arr, int start, int end) {
	if (start > end) {
		return NULL;
	}
	SegmentTree tree = (SegmentTree)malloc(sizeof(struct TNode));
	tree->start = start;
	tree->end = end;
	if (start == end) {
		tree->max_ = tree->min_ = arr[start];
		tree->left = tree->right = NULL;
	}
	else {
		int mid = (start + end) / 2;
		tree->left = buildTree(arr, start, mid);
		tree->right = buildTree(arr, mid + 1, end);
		tree->max_ = max(tree->left->max_, tree->right->max_);
		tree->min_ = min(tree->left->min_, tree->right->min_);		
	}
	return tree;
}

void query(SegmentTree tree, int start, int end, int * p_max, int * p_min) {
	if (tree == NULL) return;
	if (start <= tree->start && tree->end <= end) {
		*p_max = tree->max_;
		*p_min = tree->min_;
		return;
	}
	int mid = (tree->start + tree->end) / 2;	
	*p_max = MIN_H, *p_min = MAX_H;
	if (start <= mid) {
		query(tree->left, start, end, p_max, p_min);		
	}
	if (end > mid) {
		int tmp_max, tmp_min;
		query(tree->right, start, end, &tmp_max, &tmp_min);
		if (tmp_max > *p_max) *p_max = tmp_max;
		if (tmp_min < *p_min) *p_min = tmp_min;
	}
}

void deleteTree(SegmentTree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
}
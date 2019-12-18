#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef struct TNode {
	int start, end;
	int min;
	int tag;
	struct TNode *left, *right;
}*SegmentTree;

SegmentTree build(int * arr, int start, int end);
void update(SegmentTree st, int start, int end, int inc);
int query(SegmentTree st, int start, int end);
void tagDown(SegmentTree st);
bool contain(int start, int end, SegmentTree st);
bool cross(int start, int end, SegmentTree st);
void deleteTree(SegmentTree st);

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	int * arr = (int *)malloc(n * sizeof(int));
	int i;
	for (i = 0; i < n; i++) 
		scanf("%d", arr + i);
	SegmentTree st = build(arr, 0, n - 1);
	int d, s, t;
	bool ans = true;
	for (i = 0; i < m; i++) {
		scanf("%d %d %d", &d, &s, &t);
		s--; t--;
		update(st, s, t, 0-d);
		if (query(st, s, t) < 0) {
			ans = false;
			break;
		}
	}
	if (ans) printf("0");
	else printf("-1\n%d", i + 1);
	free(arr);
	deleteTree(st);
	return 0;
}

SegmentTree build(int * arr, int start, int end) {
	SegmentTree st = (SegmentTree)malloc(sizeof(struct TNode));
	st->start = start;
	st->end = end;
	st->tag = 0;
	if (start == end) {
		st->min = arr[start];
		st->left = st->right = NULL;
	}
	else {
		int mid = (start + end) >> 1;
		st->left = build(arr, start, mid);
		st->right = build(arr, mid + 1, end);
		st->min = min(st->left->min, st->right->min);
	}
	return st;
}

void update(SegmentTree st, int start, int end, int inc) {
	if (contain(start, end, st)) {
		st->min += inc;
		st->tag += inc;
		return;
	}
	tagDown(st);
	if (cross(start, end, st->left)) {
		update(st->left, start, end, inc);
	}
	if (cross(start, end, st->right)) {
		update(st->right, start, end, inc);
	}
	st->min = min(st->left->min, st->right->min);
}

int query(SegmentTree st, int start, int end) {
	if (contain(start, end, st)) {
		return st->min;
	}
	tagDown(st);
	int result = 0x7fffffff;
	if (cross(start, end, st->left)) {
		result = min(result, query(st->left, start, end));
	}
	if (cross(start, end, st->right)) {
		result = min(result, query(st->right, start, end));
	}
	return result;
}

void tagDown(SegmentTree st) {
	if (st->tag) {
		st->left->min += st->tag;
		st->left->tag += st->tag;
		st->right->min += st->tag;
		st->right->tag += st->tag;
		st->tag = 0;
	}
}

bool contain(int start, int end, SegmentTree st) {
	return start <= st->start && st->end <= end;
}

bool cross(int start, int end, SegmentTree st) {
	return start <= st->end && st->start <= end;
}

void deleteTree(SegmentTree st) {
	if (st == NULL) {
		return;
	}
	deleteTree(st->left);
	deleteTree(st->right);
	free(st);
}

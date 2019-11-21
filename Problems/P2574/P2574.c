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
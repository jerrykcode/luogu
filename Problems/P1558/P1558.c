#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

typedef struct TNode {
	int start, end;
	int bitmap;
	int lazy_tag;
	struct TNode *left, *right;
} *SegmentTree;

#define NO_LAZY_TAG 0

SegmentTree build(int start, int end, int init_bitmap);
void update(int start, int end, SegmentTree st, int bitmap);
int getBitmap(int start, int end, SegmentTree st);
void tagDown(SegmentTree st);
bool contain(int start, int end, SegmentTree st);
bool cross(int start, int end, SegmentTree st);
void deleteTree(SegmentTree st);

int main() {
	int l, t, o;
	scanf("%d %d %d", &l, &t, &o);
	SegmentTree st = build(0, l - 1, 2); // 1 << 1 = 2
	char cmd;
	int a, b, c;
	for (int i = 0; i < o; i++) {
		scanf(" %c %d %d", &cmd, &a, &b);
		a--; b--;
		if (a > b) {
			int tmp = a;
			a = b;
			b = tmp;
		}
		if (cmd == 'C') {
			scanf("%d", &c);
			update(a, b, st, 1 << c);
		}
		else {
			int bitmap = getBitmap(a, b, st);
			int result = 0;
			for (int i = 1; i <= t; i++) {
				if (bitmap & (1 << i)) result++;
			}
			printf("%d\n", result);
		}
	}
	deleteTree(st);
	return 0;
}

SegmentTree build(int start, int end, int init_bitmap) {
	if (start > end) return NULL;
	SegmentTree st = (SegmentTree)malloc(sizeof(struct TNode));
	st->start = start;
	st->end = end;
	st->bitmap = init_bitmap;
	st->lazy_tag = NO_LAZY_TAG;
	if (start == end) {
		st->left = st->right = NULL;
	}
	else {
		int mid = (start + end) >> 1;
		st->left = build(start, mid, init_bitmap);
		st->right = build(mid + 1, end, init_bitmap);
	}
	return st;
}

void update(int start, int end, SegmentTree st, int bitmap) {
	if (contain(start, end, st)) {
		st->bitmap = bitmap;
		st->lazy_tag = bitmap;
		return;
	}
	tagDown(st);
	if (cross(start, end, st->left)) {
		update(start, end, st->left, bitmap);
	}
	if (cross(start, end, st->right)) {
		update(start, end, st->right, bitmap);
	}
	st->bitmap = st->left->bitmap | st->right->bitmap;
}

int getBitmap(int start, int end, SegmentTree st) {
	if (contain(start, end, st)) {
		return st->bitmap;
	}
	tagDown(st);
	int result = 0;
	if (cross(start, end, st->left)) {
		result |= getBitmap(start, end, st->left);
	}
	if (cross(start, end, st->right)) {
		result |= getBitmap(start, end, st->right);
	}
	return result;
}

void tagDown(SegmentTree st) {
	if (st->lazy_tag == NO_LAZY_TAG) return;
	st->left->bitmap
		= st->left->lazy_tag
		= st->right->bitmap
		= st->right->lazy_tag
		= st->lazy_tag;
	st->lazy_tag = NO_LAZY_TAG;
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
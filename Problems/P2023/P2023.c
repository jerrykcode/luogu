#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

typedef long long LL;
LL p;

typedef struct TNode {
	int start, end;
	LL sum;
	LL mul_tag, add_tag;
	struct TNode *left, *right;
} *SegmentTree;

int len(SegmentTree st) {
	return st->end - st->start + 1;
}

SegmentTree buildTree(LL * arr, int start, int end) {
	if (start > end) return NULL;
	SegmentTree st = (SegmentTree)malloc(sizeof(struct TNode));
	st->start = start;
	st->end = end;
	if (start == end) {
		st->sum = arr[start] % p;
		st->left = st->right = NULL;
	}
	else {
		int mid = (start + end) >> 1;
		st->left = buildTree(arr, start, mid);
		st->right = buildTree(arr, mid + 1, end);
		st->sum = st->left->sum + st->right->sum;
		st->sum %= p;
	}
	st->mul_tag = 1;
	st->add_tag = 0;
	return st;
}

void tagDown(SegmentTree st) {
	if (st->mul_tag != 1 || st->add_tag != 0) {
		if (st->left) {
			st->left->sum = st->left->sum * st->mul_tag + st->add_tag * len(st->left);
			st->left->sum %= p;
			st->left->mul_tag *= st->mul_tag;
			st->left->mul_tag %= p;
			st->left->add_tag *= st->mul_tag;
			st->left->add_tag += st->add_tag;
			st->left->add_tag %= p;
		}
		if (st->right) {
			st->right->sum = st->right->sum * st->mul_tag + st->add_tag * len(st->right);
			st->right->sum %= p;
			st->right->mul_tag *= st->mul_tag;
			st->right->mul_tag %= p;
			st->right->add_tag *= st->mul_tag;
			st->right->add_tag += st->add_tag;
			st->right->add_tag %= p;
		}
		st->mul_tag = 1;
		st->add_tag = 0;
	}
}

bool contain(int start, int end, SegmentTree st) {
	return start <= st->start && st->end <= end;
}

bool cross(int start, int end, SegmentTree st) {
	return end >= st->start && start <= st->end;
}

void updateAdd(int start, int end, SegmentTree st, LL add_num) {
	if (contain(start, end, st)) {
		st->sum += add_num * len(st);
		st->sum %= p;
		st->add_tag += add_num;
		st->add_tag %= p;
		return;
	}
	tagDown(st);
	if (st->left && cross(start, end, st->left)) {
		updateAdd(start, end, st->left, add_num);
	}
	if (st->right && cross(start, end, st->right)) {
		updateAdd(start, end, st->right, add_num);
	}
	st->sum = st->left->sum + st->right->sum;
	st->sum %= p;
}

void updateMul(int start, int end, SegmentTree st, LL mul_num) {
	if (contain(start, end, st)) {
		st->sum *= mul_num;
		st->sum %= p;
		st->mul_tag *= mul_num;
		st->mul_tag %= p;
		st->add_tag *= mul_num;
		st->add_tag %= p;
		return;
	}
	tagDown(st);
	if (st->left && cross(start, end, st->left)) {
		updateMul(start, end, st->left, mul_num);
	}
	if (st->right && cross(start, end, st->right)) {
		updateMul(start, end, st->right, mul_num);
	}
	st->sum = st->left->sum + st->right->sum;
	st->sum %= p;
}

LL sum(int start, int end, SegmentTree st) {
	if (contain(start, end, st)) {
		return st->sum % p;
	}
	tagDown(st);
	LL result = 0;
	if (st->left && cross(start, end, st->left)) {
		result += sum(start, end, st->left);
	}
	if (st->right && cross(start, end, st->right)) {
		result += sum(start, end, st->right);
	}
	return result % p;
}

void deleteTree(SegmentTree st) {
	if (st == NULL) return;
	deleteTree(st->left);
	deleteTree(st->right);
	free(st);
}

int main() {
	int n, m, cmd, t, g;
	LL c;
	scanf("%d %lld", &n, &p);
	LL * arr = (LL *)malloc(n * sizeof(LL));
	for (int i = 0; i < n; i++) {
		scanf("%lld", arr + i);
	}
	SegmentTree st = buildTree(arr, 0, n - 1);
	free(arr);
	scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &cmd, &t, &g);
		t--; g--;
		if (cmd != 3) {
			scanf("%lld", &c);
			cmd == 1 ? updateMul(t, g, st, c) : updateAdd(t, g, st, c);
		}
		else printf("%lld\n", sum(t, g, st));
	}
	deleteTree(st);
	return 0;
}
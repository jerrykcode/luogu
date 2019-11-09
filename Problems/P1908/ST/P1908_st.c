#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

struct node {
	int val;
	int input_rank;
};

int compare(const void * a, const void * b) {
	//val
	return ((struct node *)a)->val - ((struct node *)b)->val;
}

typedef struct TNode {
	int start, end;
	int sum;
	struct TNode *left, *right;
} *SegmentTree;

SegmentTree buildTree(int start, int end);
int sum(int start, int end, SegmentTree st);
void update(int index, int inc, SegmentTree st);
void deleteTree(SegmentTree st);

int main() {
	int n;
	scanf("%d", &n);
	struct node * node_array = (struct node *)malloc(n * sizeof(struct node));
	for (int i = 0; i < n; i++) {
		scanf("%d", &node_array[i].val);
		node_array[i].input_rank = i;
	}
	qsort(node_array, n, sizeof(struct node), compare);
	int * rank = (int *)malloc(n * sizeof(int));
	rank[node_array[0].input_rank] = 0;
	int max = 0;
	for (int i = 1; i < n; i++) {
		if (node_array[i].val != node_array[i - 1].val) {
			rank[node_array[i].input_rank] = i;
			max = i;
		}
		else {
			rank[node_array[i].input_rank] = rank[node_array[i - 1].input_rank];
		}
	}
	free(node_array);
	SegmentTree st = buildTree(0, max);
	long long result = 0;
	for (int i = 0; i < n; i++) {
		result += sum(rank[i] + 1, max, st);
		update(rank[i], 1, st);
	}
	printf("%lld", result);
	free(rank);
	deleteTree(st);
	return 0;
}

SegmentTree buildTree(int start, int end) {
	SegmentTree st = (SegmentTree)malloc(sizeof(struct TNode));
	st->start = start;
	st->end = end;
	st->sum = 0;
	if (start != end) {
		int mid = (start + end) >> 1;
		st->left = buildTree(start, mid);
		st->right = buildTree(mid + 1, end);
	}
	else {
		st->left = st->right = NULL;
	}
	return st;
}

bool contain(int start, int end, SegmentTree st) {
	return start <= st->start && st->end <= end;
}

bool cross(int start, int end, SegmentTree st) {
	return start <= st->end && end >= st->start;
}

int sum(int start, int end, SegmentTree st) {
	if (contain(start, end, st)) {
		return st->sum;
	}
	int result = 0;
	if (st->left && cross(start, end, st->left)) {
		result += sum(start, end, st->left);
	}
	if (st->right && cross(start, end, st->right)) {
		result += sum(start, end, st->right);
	}
	return result;
}

void update(int index, int inc, SegmentTree st) {
	st->sum += inc;
	if (st->start == st->end) {
		return;
	}
	if (index <= st->left->end) {
		update(index, inc, st->left);
	}
	else {
		update(index, inc, st->right);
	}
}

void deleteTree(SegmentTree st) {
	if (st == NULL) return;
	deleteTree(st->left);
	deleteTree(st->right);
	free(st);
}
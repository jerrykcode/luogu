#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef struct Range {
	int start, end;
	int len;
} Range;

int compareInt(const void * a, const void * b) {
	return*(int *)a - *(int *)b;
}

int compareRange(const void * a, const void * b) {
	return ((Range *)a)->len - ((Range *)b)->len;
}

int binarySearch(int * arr, int n, int val);

typedef struct TNode {
	int start, end;
	int max;
	int lazy;
	struct TNode *left, *right;
} *SegmentTree;

SegmentTree buildTree(int start, int end);
void update(SegmentTree st, int start, int end, int inc);
bool contain (int start, int end, SegmentTree st);
bool cross(int start, int end, SegmentTree st);
void deleteTree(SegmentTree st);

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	Range * ranges = (Range *)malloc(n * sizeof(Range));
	int * arr = (int *)malloc(2 * n * sizeof(int));
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &ranges[i].start, &ranges[i].end);
		ranges[i].len = ranges[i].end - ranges[i].start;
		arr[i << 1] = ranges[i].start; // i << 1 = i * 2
		arr[(i << 1) | 1] = ranges[i].end; // (i << 1) | 1 = i * 2 + 1
	}
	qsort(arr, 2 * n, sizeof(int), compareInt);
	for (int i = 0; i < n; i++) {
		ranges[i].start = binarySearch(arr, 2 * n, ranges[i].start);
		ranges[i].end = binarySearch(arr, 2 * n, ranges[i].end);
	}
	free(arr);
	qsort(ranges, n, sizeof(Range), compareRange);
	SegmentTree st = buildTree(0, 2 * n - 1);
	int ans = 0x7fffffff;
	int last = 0;
	for (int i = 0; i < n; i++) {
		update(st, ranges[i].start, ranges[i].end, 1);
		while (m == st->max) {
			ans = min(ans, ranges[i].len - ranges[last].len);
			update(st, ranges[last].start, ranges[last].end, -1);
			last++;
		}
	}
	if (ans == 0x7fffffff) ans = -1;
	printf("%d", ans);
	free(ranges);
	deleteTree(st);
	return 0;
}

int binarySearch(int * arr, int n, int val) {
	int left = 0, right = n - 1;
	while (left <= right) {
		int mid = (left + right) >> 1;
		if (arr[mid] == val) {
			return mid;
		}
		else if (arr[mid] > val) {
			right = mid - 1;
		}
		else { //arr[mid] < val
			left = mid + 1;
		}
	}
	return -1;
}

SegmentTree buildTree(int start, int end) {
	if (start > end) return NULL;
	SegmentTree st = (SegmentTree)malloc(sizeof(struct TNode));
	st->start = start;
	st->end = end;
	st->max = 0;
	st->lazy = 0;
	if (start == end) {
		st->left = st->right = NULL;
	}
	else {
		int mid = (start + end) >> 1;
		st->left = buildTree(start, mid);
		st->right = buildTree(mid + 1, end);
	}
	return st;
}

void update(SegmentTree st, int start, int end, int inc) {
	if (contain(start, end, st)) {
		st->max += inc;
		st->lazy += inc;
		return;
	}
	if (st->lazy) {
		st->left->max += st->lazy;
		st->left->lazy += st->lazy;
		st->right->max += st->lazy;
		st->right->lazy += st->lazy;
		st->lazy = 0;
	}
	if (cross(start, end, st->left)) {
		update(st->left, start, end, inc);
	}
	if (cross(start, end, st->right)) {
		update(st->right, start, end, inc);
	}
	st->max = max(st->left->max, st->right->max);
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
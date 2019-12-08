#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

typedef unsigned char byte;
#define NO_LAZY 0xff

struct Segment {
	int start, end;
	byte lazy;
	int sum;
};

int left(int id) { return id << 1 | 1; }
int right(int id) { return left(id) + 1; }

struct Segment * segmentTree;
int * arr;

void build(int id, int start, int end, int midval);
void update(int id, int start, int end, int val);
int queryRange(int id, int start, int end);
int queryIndex(int id, int index);
void tagDown(int id);
bool contain(int start, int end, int id);
bool cross(int start, int end, int id);
int len(int id);

struct Option {
	bool increase;
	int start, end;
};

int main() {
	int n, m, q, i;
	scanf("%d %d", &n, &m);
	arr = (int *)malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		scanf("%d", arr + i);
	}
	segmentTree = (struct Segment *)malloc(4 * n * sizeof(struct Segment));
	struct Option * opts = (struct Option *)malloc(m * sizeof(struct Option));
	int op, l, r;
	for (i = 0; i < m; i++) {
		scanf("%d %d %d", &op, &l, &r);
		opts[i].increase = !op;
		opts[i].start = l - 1;
		opts[i].end = r - 1;
	}
	scanf("%d", &q);
	q--;
	l = 1, r = n;
	int mid, ones, result;
	while (l <= r) {
		mid = (l + r) >> 1;
		build(0, 0, n - 1, mid);
		for (i = 0; i < m; i++) {
			ones = queryRange(0, opts[i].start, opts[i].end);
			if (opts[i].increase) {
				update(0, opts[i].start, opts[i].end - ones, 0);
				update(0, opts[i].end -ones + 1, opts[i].end, 1);
			}
			else {
				update(0, opts[i].start, opts[i].start + ones - 1, 1);
				update(0, opts[i].start + ones, opts[i].end, 0);
			}
		}
		if (queryIndex(0, q)) {
			result = mid;
			l = mid + 1;
		}
		else r = mid - 1;
	}
	printf("%d", result);
	free(segmentTree);
	free(arr);
	free(opts);
	return 0;
}

void build(int id, int start, int end, int midval) {
	segmentTree[id].start = start;
	segmentTree[id].end = end;
	segmentTree[id].lazy = NO_LAZY;
	if (start == end) {
		segmentTree[id].sum = arr[start] >= midval;
		return;
	}
	int mid = (start + end) >> 1;
	build(left(id), start, mid, midval);
	build(right(id), mid + 1, end, midval);
	segmentTree[id].sum = segmentTree[left(id)].sum + segmentTree[right(id)].sum;
}


void update(int id, int start, int end, int val) {
	if (contain(start, end, id)) {
		segmentTree[id].sum = len(id) * val;
		segmentTree[id].lazy = val;
		return;
	}
	tagDown(id);
	if (cross(start, end, left(id))) {
		update(left(id), start, end, val);
	}
	if (cross(start, end, right(id))) {
		update(right(id), start, end, val);
	}
	segmentTree[id].sum = segmentTree[left(id)].sum + segmentTree[right(id)].sum;
}

int queryRange(int id, int start, int end) {
	if (contain(start, end, id)) {
		return segmentTree[id].sum;
	}
	tagDown(id);
	int result = 0;
	if (cross(start, end, left(id))) {
		result += queryRange(left(id), start, end);
	}
	if (cross(start, end, right(id))) {
		result += queryRange(right(id), start, end);
	}
	return result;
}

int queryIndex(int id, int index) {
	if (segmentTree[id].start == segmentTree[id].end) {
		return segmentTree[id].sum;
	}
	tagDown(id);
	if (segmentTree[left(id)].end >= index) {
		return queryIndex(left(id), index);
	}
	else {
		return queryIndex(right(id),index);
	}
}

void tagDown(int id) {
	if (segmentTree[id].lazy != NO_LAZY) {
		segmentTree[left(id)].sum = len(left(id)) * segmentTree[id].lazy;
		segmentTree[left(id)].lazy = segmentTree[id].lazy;
		segmentTree[right(id)].sum = len(right(id)) * segmentTree[id].lazy;
		segmentTree[right(id)].lazy = segmentTree[id].lazy;
		segmentTree[id].lazy = NO_LAZY;
	}
}

bool contain(int start, int end, int id) {
	return start <= segmentTree[id].start && segmentTree[id].end <= end;
}

bool cross(int start, int end, int id) {
	return start <= segmentTree[id].end && segmentTree[id].start <= end;
}

int len(int id) {
	return segmentTree[id].end - segmentTree[id].start + 1;
}

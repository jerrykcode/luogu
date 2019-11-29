# P1712 [NOI2016]区间

[原题地址](https://www.luogu.com.cn/problem/P1712)

## 思路

### 离散化

```c
typedef struct Range {
	int start, end;
	int len;
} Range;
```
start, end表示一个区间开始和结束的端点，len = end - start。start，end，len在输入时即可确定。  
离散化后，start，end更新为新值，而len不要变化，因为最后的答案是更具原来的区间长度进行计算的。  

如何离散化  
输入时使用一个数组存储所有区间的start和end，然后对该数组排序。之后对每个区间，将其start和end分别更新为start，end在数组中的下标(二分搜索下标)。  

### 线段树
参考[AcFirmament 的博客](https://www.luogu.com.cn/blog/AcFcAcFcA/solution-p1712):

>先按照长度排个序，然后依次添加区间。什么是添加？设这个区间是[l,r]，添加就是把a[l], a[l+1], a[l+2], ..., a[r]都加上1，其中a[i]表示第i个位置被几个区间覆盖。拿走一个区间的含义就是把它们都减1。这个过程很显然可以用线段树维护。  
如果在添加到一个区间i时，有一个点被区间覆盖了M次，那么先更新答案，再把前面的加入过的区间一直拿直到没有一个点被覆盖M次。如何判断有没有点被覆盖M次？因为是一个一个区间加的，所以只用维护一个a[i]的最大值，看他是否=M就行了。  
什么叫再把前面的加入过的区间一直拿直到没有一个点被覆盖M次？  
比如你一直添加区间到第5个，此时有一个点被覆盖了M次。这时你就将第一个区间拿出，如果此时依然有有一个点被覆盖了M次，那么你就拿走第二个...  
这个过程就好比一个队列，可以从后面添加区间达到一个点被覆盖了M次；从前面弹出区间直到没有一个点被覆盖了M次。

注意线段树维护一段的最大值

## 代码

```c
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
```
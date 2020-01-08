#include "stdio.h"
#include "stdlib.h"

#define maxn 100000000

struct node {
	int left, right;
	int val;
} tree[maxn];
int top = 0;

int build(int start, int end, int * arr) {
	int node = top++;
	if (start == end) {
		tree[node].val = arr[start];		
	}
	else {
		int mid = (start + end) >> 1;
		tree[node].left = build(start, mid, arr);
		tree[node].right = build(mid + 1, end, arr);
	}
	return node;
}

int update(int node, int start, int end, int index, int val) {
	int node_copy = top++;
	tree[node_copy] = tree[node];
	if (start == end) {
		tree[node_copy].val = val;
	}
	else {
		int mid = (start + end) >> 1;
		if (index <= mid)
			tree[node_copy].left = update(tree[node].left, start, mid, index, val);
		else
			tree[node_copy].right = update(tree[node].right, mid + 1, end, index, val);
	}
	return node_copy;
}

int query(int node, int start, int end, int index) {
	if (start == end) return tree[node].val;
	int mid = (start + end) >> 1;
	if (index <= mid)
		return query(tree[node].left, start, mid, index);
	else
		return query(tree[node].right, mid + 1, end, index);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	int * arr = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		scanf("%d", arr + i);
	int node = build(0, n - 1, arr);
	free(arr);
	int * roots = (int *)malloc((m + 1) * sizeof(int));
	roots[0] = node;
	int version, opt, index, val;
	for (int i = 1; i <= m; i++) {
		scanf("%d %d %d", &version, &opt, &index);
		index--;
		if (opt == 1) {
			scanf("%d", &val);
			roots[i] = update(roots[version], 0, n - 1, index, val);
		}
		else {
			printf("%d\n", query(roots[version], 0, n - 1, index));
			roots[i] = roots[version];
		}
	}
	free(roots);
	return 0;
}

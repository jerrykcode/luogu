#include "stdio.h"
#include "stdlib.h"

#define MAX_NUM 1000000
int pos[MAX_NUM + 1];
int n, m;
int *num, *tree;
struct Query {
	int left, right;
	int index;
};
struct Query * queries;
int * answer;

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (; x <= n; x += lowbit(x)) {
		tree[x] += k;
	}
}

int sum(int x) {
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += tree[x];
	}
	return result;
}

int compare(const void * a, const void * b) {
	return ((struct Query *)a)->right - ((struct Query *)b)->right;
}

int main() {
	memset(pos, 0, sizeof(pos));
	scanf("%d", &n);
	num = (int *)malloc((n + 1) * sizeof(int));
	for (int i = 1; i < n + 1; i++) {
		scanf("%d", num + i);
	}
	tree = (int *)malloc((n + 1) * sizeof(int));
	memset(tree, 0, (n + 1) * sizeof(int));
	scanf("%d", &m);
	queries = (struct Query *)malloc(m * sizeof(struct Query));	
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &queries[i].left, &queries[i].right);
		queries[i].index = i;
	}
	answer = (int *)malloc(m * sizeof(int));
	qsort(queries, m, sizeof(struct Query), compare);
	for (int i = 0, j = 1; i < m; i++) {
		for (; j <= queries[i].right; j++) {
			if (pos[num[j]]) add(pos[num[j]], -1);
			add(j, 1);
			pos[num[j]] = j;
		}
		answer[queries[i].index] = sum(queries[i].right) - sum(queries[i].left - 1);
	}
	for (int i = 0; i < m; i++) {
		printf("%d\n", answer[i]);
	}
	free(num);
	free(tree);
	free(queries);
	free(answer);
	return 0;
}
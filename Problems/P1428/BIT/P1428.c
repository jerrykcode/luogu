#include "stdio.h"
#include "stdlib.h"

int * c, n;

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (; x <= n; x += lowbit(x)) {
		c[x] += k;
	}
}

int sum(int x) {
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += c[x];
	}
	return result;
}

struct node {
	int val;
	int input_rank;
};

int compare(const void * a, const void * b) {
	return ((struct node *)a)->val != ((struct node *)b)->val ? ((struct node *)a)->val - ((struct node *)b)->val : 
		((struct node *)b)->input_rank - ((struct node *)a)->input_rank;
}

int main() {
	scanf("%d", &n);
	struct node * node_array = (struct node *)malloc(n * sizeof(struct node));
	int i = 0, val;
	for (; i < n; i++) {
		scanf("%d", &val);
		node_array[i].val = val;
		node_array[i].input_rank = i;
	}
	qsort(node_array, n, sizeof(struct node), compare);
	int * rank = (int *)malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		rank[node_array[i].input_rank] = i + 1;
	}
	free(node_array);	
	c = (int *)malloc((n + 1) * sizeof(int));
	for (i = 1; i < n + 1; i++) c[i] = 0;
	for (i = 0; i < n; i++) {	
		if (i != 0) putchar(' ');
		printf("%d", sum(rank[i] - 1));
		add(rank[i], 1);
	}
	free(rank);
	free(c);
	return 0;
}
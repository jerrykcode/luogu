#include "stdio.h"
#include "stdlib.h"

int * c, n;

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (int i = x; i <= n; i += lowbit(i)) {
		c[i] += k;
	}
}

int sum(int x) {
	int result = 0;
	for (int i = x; i > 0; i -= lowbit(i)) {
		result += c[i];
	}
	return result;
}

int main() {
	int m, i, cmd, x, y;
	scanf("%d %d", &n, &m);
	c = (int *)malloc((n + 1) * sizeof(int));
	for (i = 1; i <= n; i++) c[i] = 0;
	for (i = 1; i <= n; i++) {
		scanf("%d", &x);
		add(i, x);
	}
	for (i = 0; i < m; i++) {
		scanf("%d %d %d", &cmd, &x, &y);
		if (cmd == 1) {
			add(x, y);
		}
		else {
			printf("%d\n", sum(y) - sum(x - 1));
		}
	}
	free(c);
	return 0;
}
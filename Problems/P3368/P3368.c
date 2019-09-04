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

int main() {
	int m, i, cmd, x, y, k, tmp = 0;
	scanf("%d %d", &n, &m);
	c = (int *)malloc((n + 1) * sizeof(int));
	for (i = 1; i <= n; i++) c[i] = 0;
	for (i = 1; i <= n; i++) {
		scanf("%d", &x);
		if (i != 1) {
			x -= tmp;
		}
		tmp += x;
		add(i, x);
	}
	for (i = 0; i < m; i++) {
		scanf("%d", &cmd);
		if (cmd == 1) {
			scanf("%d %d %d", &x, &y, &k);
			add(x, k);
			add(y + 1, -k);
		}
		else {
			scanf("%d", &x);
			printf("%d\n", sum(x));
		}
	}
	free(c);
	return 0;
}
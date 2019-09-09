#include "stdio.h"
#include "stdlib.h"

int * c, n;

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (; x <= n; x += lowbit(x))
		c[x - 1] += k;  //c从下标0开始使用
}

int sum(int x) {
	int result = 0;
	for (; x > 0; x -= lowbit(x))
		result += c[x - 1];  //c从下标0开始使用
	return result;
}

int range_sum(int x, int y) {
	return sum(y) - sum(x - 1);
}

int main() {
	scanf("%d", &n);
	c = (int *)malloc(n * sizeof(int)); //c从下标0开始使用
	for (int i = 0; i < n; i++) c[i] = 0;
	int w;
	scanf("%d", &w);
	char cmd;
	int a, b;
	for (int i = 0; i < w; i++) {
		scanf(" %c %d %d", &cmd, &a, &b);
		if (cmd == 'x') {
			add(a, b);
		}
		else {
			printf("%d\n", range_sum(a, b));
		}
	}
	free(c);
	return 0;
}
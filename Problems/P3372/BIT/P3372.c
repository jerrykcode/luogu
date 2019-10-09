#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef long long LL;

//树状数组模板
int lowbit(int x) {
	return x & -x;
}

void add(LL * c, int n, int x, LL k) {
	for (; x <= n; x += lowbit(x)) {
		c[x] += k;
	}
}

LL sum(LL * c, int x) {
	LL result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += c[x];
	}
	return result;
}

//使用两个树状数组c1, c2
//详见https://www.cnblogs.com/dilthey/p/9366491.html#c
LL *c1, *c2;
int n;

void range_add(int start, int end, LL k) {
	add(c1, n, start, k);
	add(c1, n, end + 1, -k);
	add(c2, n, start, k * start);
	add(c2, n, end + 1, -k * (end + 1));
}

LL sum1(int x) {
	return (x + 1) * sum(c1, x) - sum(c2, x);
}

LL range_sum(int start, int end) {
	return sum1(end) - sum1(start - 1);
}

int main() {
	int m;
	scanf("%d %d", &n, &m);
	size_t size = (n + 1) * sizeof(LL);
	c1 = (LL *)malloc(size);
	c2 = (LL *)malloc(size);
	memset(c1, 0, size);
	memset(c2, 0, size);
	LL pre = 0, val;
	for (int i = 1; i <= n; i++) {
		scanf("%lld", &val);
		add(c1, n, i, val - pre);
		add(c2, n, i, (val - pre) * i);
		pre = val;
	}
	int cmd, x, y;
	LL k;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &cmd, &x, &y);
		if (cmd == 1) {
			scanf("%lld", &k);
			range_add(x, y, k);
		}
		else {
			printf("%lld\n", range_sum(x, y));
		}
	}
	free(c1);
	free(c2);
	return 0;
}

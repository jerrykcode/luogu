#include "stdio.h"
#include "stdlib.h"
#define N 13
long long f[N], ten[N];
long long ans1[10], ans2[10];
unsigned char val[N];
static inline void init() {
	f[0] = 0;
	ten[0] = 1;
	for (int i = 1; i < N; i++) {
		f[i] = 10 * f[i - 1];
		f[i] += ten[i - 1];
		ten[i] = ten[i - 1] * 10;
	}
}
static inline void solve(long long x, long long *ans) {
	int n = 0;
	long long tmp = x;
	while (x) {
		val[++n] = x % 10;
		x /= 10;
	}
	for (int i = 0; i < 10; i++) ans[i] = 0;
	for (int i = n; i >= 1; i--) {
		for (int j = 0; j < 10; j++) ans[j] += f[i - 1] * val[i];
		for (int j = 0; j < val[i]; j++) ans[j] += ten[i - 1];
		tmp -= val[i] * ten[i - 1];
		//printf("[%d] tmp: %lld\n", i, tmp);
		ans[val[i]] += tmp + 1;
		ans[0] -= ten[i - 1];
	}
}
int main() {
	long long a, b;
	scanf("%lld %lld", &a, &b);
	init();
	solve(a - 1, ans1);
	solve(b, ans2);
	for (int i = 0; i < 10; i++) {
		if (i) putchar(' ');
		printf("%lld", ans2[i] - ans1[i]);
	}
	exit(0);
}

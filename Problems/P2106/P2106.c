#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MOD 1000000007
static inline void mat_mul(int (*des)[10], int rows /* <= 10*/, int src[10][10]) {
	static int tmp[10][10];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < 10; j++) {
			tmp[i][j] = 0;
			for (int k = 0; k < 10; k++)
				tmp[i][j] = (tmp[i][j] + (des[i][k] * 1ll * src[k][j]) % MOD ) % MOD;
		}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < 10; j++)
			des[i][j] = tmp[i][j];
}
static inline int max(int a, int b) { return a > b ? a : b; }
int main() {
	long long k;
	int ans;
	scanf("%lld", &k);
	if (k == 1) { printf("10"); return 0; }
	k--;
	int dp[1][10], base[10][10];
	for (int i = 0; i < 10; i++) dp[0][i] = 1;
	memset(base, 0, sizeof(base));
	for (int i = 0; i < 10; i++)
		for (int j = max(i - 2, 0); j <= i + 2 && j < 10; j++)
			base[i][j] = 1;
	while (k) {
		if (k & 1) {
			mat_mul(dp, 1, base);
		}
		mat_mul(base, 10, base);
		k >>= 1;
	}
	ans = 0;
	for (int i = 1; i < 10; i++) ans = (ans + dp[0][i]) % MOD;
	printf("%d", ans);
	exit(0);
}

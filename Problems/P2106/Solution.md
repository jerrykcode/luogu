[原题地址](https://www.luogu.com.cn/problem/P2106)
题意：求k阶sam数（长度为k位，相邻两位数字之差不超过2的数）的数量

$$ dp_{i,j} 表示长度i位最高位为j的符合条件的数字数量$$

$$
dp_{i,j} = \sum_{k=\max(0,j-2)}^{\min(9,j+2)} dp_{i-1,k}
$$
使用矩阵加速递推。初始状态组向量×矩阵的k-1次方得到的向量包含最终答案所需k阶最高位为各个数的sam数数量。矩阵乘方用快速幂计算。
$$
F(i) = \begin{bmatrix}
dp_{i,0} & dp_{i,1} & dp_{i,2} & dp_{i,3} & \dots & dp_{i,9}
\end{bmatrix}
$$

$$
F(i+1) = \begin{bmatrix}
dp_{i+1,0} & dp_{i+1,1} & dp_{i+1,2} & dp_{i+1,3} & \dots & dp_{i+1,9}
\end{bmatrix}
$$

$$
BASE = \begin{bmatrix}
1 & 1 & 1 & 0 & \dots & 0 \\
1 & 1 & 1 & 1 & \dots & 0 \\
1 & 1 & 1 & 1 & \dots & 0 \\
0 & 1 & 1 & 1 & \dots & 0 \\
0 & 0 & 1 & 1 & \dots & 0 \\
0 & 0 & 0 & 1 & \dots & 0 \\
0 & 0 & 0 & 0 & \dots & 0 \\
\vdots & \vdots & \vdots & \vdots & \ddots & \vdots \\
0 & 0 & 0 & 0 & \dots & 0 \\
0 & 0 & 0 & 0 & \dots & 1 \\
0 & 0 & 0 & 0 & \dots & 1 \\
0 & 0 & 0 & 0 & \dots & 1
\end{bmatrix}
$$

$$
F(i) \times BASE = F(i+1)
$$

$$
F(1) \times BASE^{k-1} = F(k)
$$


## Code

``` c
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

```

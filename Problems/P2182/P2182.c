#include <stdio.h>
#include <stdlib.h>

#define N 101
#define K 101
#define MOD 1000000007

int dp[K][N];
int c[N][N];

static inline int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, k, m, i, same, j, l;
    scanf("%d %d %d", &n, &k, &m);
    same = 0;
    {
        int a[N], b;
        for (i = 0; i < n; i++) scanf("%1d", a + i);
        for (i = 0; i < n; i++) {
            scanf("%1d", &b);
            if (!a[i]^b) same++;
        }
    }
    dp[0][same] = 1;
    for (i = 0; i <= n; i++) c[i][0] = 1;
    for (i = 1; i <= n; i++)
        for (j = 1; j <= i; j++) c[i][j] = (c[i - 1][j] + c[i - 1][j - 1])%MOD;
    for (i = 1; i <= k; i++)
        for (j = 0; j <= n; j++)
            for (l = max(0, m + j - n); l <= j && l <= m; l++) {
                dp[i][j + m - (l << 1)] = (dp[i][j + m - (l << 1)] + (((1ll * dp[i - 1][j] * c[j][l]) % MOD) * c[n - j][m - l]) % MOD) % MOD;
            }
    printf("%d", dp[k][n]);
    exit(0);
}

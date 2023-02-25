#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

#define N 2003

int c[N][N];
ll dp[N][N];

static inline ll min(ll a, ll b) { return a < b ? a : b;  }

int main() {
    int n, i, j, len, k;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++)
            scanf("%d", &c[i][j]);
        for (j = 0; j < i; j++)
            c[i][j] = c[j][i];
    }
    for (i = 0; i < n; i++) {
        dp[i][i] = c[i][i];
        for (j = 0; j < i; j++)
            dp[i][i] = min(dp[i][i], c[j][i - 1] + c[j][i]);
        for (j = i + 1; j < n; j++)
            dp[i][i] = min(dp[i][i], c[i][j] + c[i + 1][j]);
        for (j = 0; j < i; j++)
            for (k = i + 1; k < n; k++)
                dp[i][i] = min(dp[i][i], c[j][k] + c[j][i - 1] + c[i + 1][k]);
    }
    for (len = 2; len <= n; len++)
        for (i = 0, j = len - 1; j < n; i++, j++) {
            dp[i][j] = 0x7fffffffffffffff;  // Max long long
            for (k = i; k < j; k++) 
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
            dp[i][j] = min(dp[i][j], c[i][j] + dp[i + 1][j]);
            dp[i][j] = min(dp[i][j], c[i][j] + dp[i][j - 1]);
            for (k = i + 1; k < j; k++)
                dp[i][j] = min(dp[i][j], c[i][j] + dp[i][k - 1] + dp[k + 1][j]);
        }
    printf("%lld\n", dp[0][n - 1]);
    exit(0);
}

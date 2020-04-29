#include "stdio.h"
#include "stdlib.h"

#define maxn 1000
#define mod 2015

short dp[maxn + 1][maxn + 1];

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    dp[2][0] = dp[2][1] = 1;
    for (int i = 3; i <= n; i++) {
        dp[i][0] = 1;
        for (int j = 1; j <= k && j <= i - 1; j++) {
            dp[i][j] = dp[i - 1][j - 1] % mod;
            dp[i][j] = (dp[i][j] + dp[i - 1][j]) % mod;
            dp[i][j] = (dp[i][j] + dp[i - 1][j] * j) % mod;
            dp[i][j] = (dp[i][j] + dp[i - 1][j - 1] * (i - j - 1)) % mod;
        }
    }
    printf("%hd", dp[n][k]);
    return 0;
}

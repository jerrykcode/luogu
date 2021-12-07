#include "stdio.h"
#include "stdlib.h"

#define MAXN 1000
#define MAXM 100

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int w[MAXN], dp[MAXN + 1][MAXM + 1];

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%d", &w[i]);
    }
    dp[1][1] = w[0];
    for (int i = 2; i <= n; i++) {
        dp[i][1] = dp[i - 1][1] ^ w[i - 1];
        for (int j = 2; j <= m && j <= i; j++) {
            int ww = w[i - 1];
            for (int k = i - 1; k >= j - 1; ww ^= w[--k]) {
                dp[i][j] = MAX(dp[i][j], dp[k][j - 1] + ww);
            }
        }
    }
    printf("%d\n", dp[n][m]);
    exit(0);
}

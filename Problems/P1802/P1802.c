#include "stdio.h"
#include "stdlib.h"

#define maxx 1000

int dp[maxx + 1];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, x, lose, win, use;
    scanf("%d %d", &n, &x);
    int i, j;
    for (i = 0; i < n; i++) {
        scanf("%d %d %d", &lose, &win, &use);
        for (j = x; j >= use; j--)
            dp[j] = max(dp[j] + lose, dp[j - use] + win);
        for (j = use - 1; j >= 0; j--)
            dp[j] = dp[j] + lose;
    }
    long long ans = dp[x];
    printf("%lld", ans * 5);
    return 0;
}

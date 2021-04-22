#include <stdio.h>
#include <stdlib.h>

#define MAXN 100
#define MAXM 100

int dp[2][MAXM + 1];

#define MOD 1000007

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int cur = 0, pre = 1, a, window;
    dp[cur][0] = dp[pre][0] = 1;
    for (int i = 0; i < n; i++) {
        scanf("%d", &a);
        window = dp[pre][0];
        for (int j = 1; j <= m; j++) {
            window += dp[pre][j];
            if (j - a - 1 >= 0) window -= dp[pre][j - a - 1];
            dp[cur][j] = window % MOD;
        }
        cur += pre;
        pre = cur - pre;
        cur -= pre;
    }
    printf("%d\n", dp[pre][m]);
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>

#define MAXM 24
#define MAXN 30000

int dp[MAXM + 1][MAXN + 1];

int max(int a, int b) {return a > b ? a : b; }

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int v, p;
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &v, &p);
        p *= v;
        for (int j = 0; j < v; j++)
            dp[i][j] = dp[i - 1][j];
        for (int j = v; j <= n; j++)
            dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - v] + p);
    }
    printf("%d\n", dp[m][n]);
    exit(0);
}

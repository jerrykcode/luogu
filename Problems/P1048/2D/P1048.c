#include <stdio.h>
#include <stdlib.h>

#define MAXM 100
#define MAXT 1000
int dp[MAXM + 1][MAXT + 1]; //2D

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int t, m, ti, vi;
    scanf("%d %d", &t, &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &ti, &vi);
        for (int j = 1; j < ti; j++)
            dp[i][j] = dp[i - 1][j];
        for (int j = ti; j <= t; j++)
            dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - ti] + vi);
    }
    printf("%d\n", dp[m][t]);
    exit(0);
}

#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int value[100];
int weight[2][100];
int dp[1001][1001];

int main() {
    int n, m, x;
    scanf("%d %d %d", &n, &m, &x);
    for (int i = 0; i < n; i++)
        scanf("%d %d %d", value + i, weight[0] + i, weight[1] + i);
    for (int i = 0; i < n; i++)
        for (int j = m; j >= weight[0][i]; j--)
            for (int k = x; k >= weight[1][i]; k--) {
                dp[j][k] = max(dp[j][k], dp[j - weight[0][i]][k - weight[1][i]] + value[i]);
                if (i == n - 1) break;
            }
    printf("%d", dp[m][x]);
    return 0;
}

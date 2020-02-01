#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define maxn 2500

int matrix[maxn][maxn];
int dp[maxn][maxn];
int h[maxn][maxn];
int v[maxn][maxn];

int min(int a, int b) {
    return a < b ? a : b;
}

int main() {
    int n, m, res = 0;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &matrix[i][j]);
    dp[0][0] = matrix[0][0];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (!matrix[i][j]) {
                h[i][j] = j ? h[i][j - 1] + 1 : 1;
                v[i][j] = i ? v[i - 1][j] + 1 : 1;
            }
            else {
                if (i - 1 >= 0 && j - 1 >= 0) dp[i][j] = min(dp[i - 1][j - 1], min(h[i][j - 1], v[i - 1][j])) + 1;
                else dp[i][j] = 1;
            }
            if (dp[i][j] > res) res = dp[i][j];
        }
    memset(dp, 0, sizeof(dp));
    memset(h, 0, sizeof(h));
    memset(v, 0, sizeof(v));
    dp[0][m - 1] = matrix[0][m - 1];
    for (int i = 0; i < n; i++)
        for (int j = m - 1; j >= 0; j--) {
            if (!matrix[i][j]) {
                h[i][j] = j + 1 < m ? h[i][j + 1] + 1 : 1;
                v[i][j] = i ? v[i - 1][j] + 1 : 1;
            }
            else {
                if (i - 1 >= 0 && j + 1 < m) dp[i][j] = min(dp[i - 1][j + 1], min(h[i][j + 1], v[i - 1][j])) + 1;
                else dp[i][j] = 1;
            }
            if (dp[i][j] > res) res = dp[i][j];
        }
    printf("%d\n", res);
    return 0;
}

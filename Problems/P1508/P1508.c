#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N 202
#define MIN 0x80808080

int arr[N][N];
int dp[N][N];

static inline int max(int a, int b) { return a > b ? a : b; }
static inline int max3(int a, int b, int c) { return max(a, max(b, c)); }

int main() {
    int n, m, i, j, ans;
    scanf("%d %d", &n, &m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &arr[i][j]);
    i = n - 1;
    j = m / 2;
    memset(dp, 0x80, sizeof(dp)); // MIN 0x80808080 A very small number(negative)
    if (j - 1 >= 0) dp[i][j - 1] = arr[i][j - 1];
    dp[i][j] = arr[i][j];
    if (j + 1 < m) dp[i][j + 1] = arr[i][j + 1];
    for (i = n - 2; i >= 0; i--)
        for (j = 0; j < n; j++) {
            dp[i][j] = max3((j - 1 >= 0 ? dp[i + 1][j - 1] : MIN), dp[i + 1][j], (j + 1 < m ? dp[i + 1][j + 1] : MIN));
            if (dp[i][j] != MIN) dp[i][j] += arr[i][j];
        }
    ans = MIN;
    for (i = 0; i < m; i++)
        ans = max(ans, dp[0][i]);
    printf("%d", ans);
    exit(0);
}

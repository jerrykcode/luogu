#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int a[50];
int loc[50];
int dp[50][50][2];
int n;

int min(int a, int b) {
    return a < b ? a : b;
}

int cal(int i, int j, int l, int r) {
    return (loc[j] - loc[i]) * (a[l] + a[n - 1] - a[r - 1]);
}

int main() {
    memset(dp, 0x3f, sizeof(dp));
    int c;
    scanf("%d %d", &n, &c);
    c--;
    for (int i = 0; i < n; i++) {
        scanf("%d %d", loc + i, a + i);
        loc[i]--;
        if (i) a[i] += a[i - 1];
    }
    dp[c][c][0] = dp[c][c][1] = 0;
//    for (int j = c; j < n; j++)
//        for (int i = j - 1; i >= 0; i--) {
    for (int i = c; i >= 0; i--)
        for (int j = i + 1; j < n; j++) {
            dp[i][j][0] = min(dp[i + 1][j][0] + cal(i, i + 1, i, j + 1), 
                              dp[i + 1][j][1] + cal(i, j, i, j + 1));
            dp[i][j][1] = min(dp[i][j - 1][0] + cal(i, j, i - 1, j),
                              dp[i][j - 1][1] + cal(j - 1, j, i - 1, j));
        }
    printf("%d", min(dp[0][n - 1][0], dp[0][n - 1][1]));
    return 0;
}

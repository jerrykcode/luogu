#include "stdio.h"
#include "stdlib.h"

#define maxn 100000
#define maxm 500

int dp[maxn + 1][maxm + 2][2];

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, m, d;
    scanf("%d %d", &n, &m);
    scanf("%d", &d);
    dp[1][1][1] = d;
    for (int i = 2; i <= n; i++) {
        scanf("%d", &d);
        int mm = min(i, m);
        for (int j = 0; j <= mm; j++) {
            dp[i][j][0] = max(dp[i - 1][j + 1][0], dp[i - 1][j + 1][1]);
            if (j == 0) dp[i][j][0] = max(dp[i][j][0], dp[i - 1][0][0]);
            if (j - 1 > 0) 
                dp[i][j][1] = d + dp[i - 1][j - 1][1];
            else if (j - 1 == 0) 
                dp[i][j][1] = d + dp[i - 1][0][0];
        }
    }
    printf("%d", dp[n][0][0]);
    return 0;
}

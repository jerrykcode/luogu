#include <stdio.h>
#include <stdlib.h>

#define INF 0x7fffffff
#define maxn 2001
int a[maxn];
int dp[maxn][maxn];

int main() {
    int n;
    while (scanf("%d", &n) != EOF) {
        for (int i = 1; i <= n; i++)
            scanf("%d", a + i);
        dp[1][1] = -1;
        for (int i = 2; i <= n; i++)
            for (int j = 1; j <= i; j++)
                dp[i][j] = INF;
        for (int i = 1; i < n; i++)
            for (int j = 1; j <= i; j++)
                if (dp[i][j] != INF) {
                    if (a[i] < a[i + 1]) 
                        if (dp[i][j] < dp[i + 1][j + 1]) dp[i + 1][j + 1] = dp[i][j];
                    if (dp[i][j] < a[i + 1])
                        if (a[i] < dp[i + 1][i - j + 1]) dp[i + 1][i - j + 1] = a[i];
                }
        if (dp[n][n/2] != INF) printf("Yes!\n");
        else printf("No!\n");
    }
    return 0;
}

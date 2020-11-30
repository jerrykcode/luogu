#include <stdio.h>
#include <stdlib.h>

#define maxn 101
#define maxl 202

int dp[maxl][maxn];
int e[maxn];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n;
    scanf("%d", &n);
    int l = n << 1;
    for (int i = 0; i < n; i++) {
        scanf("%d", &e[i]);
    }
    for (int len = 2; len <= n; len++)
        for (int i = 0; i < l - len + 1; i++) {
            int j = i + len - 1;
            for (int k = i; k < j; k++)
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k + 1][j] + e[i%n]*e[(k + 1)%n]*e[(j + 1)%n]);
        }
    int ans = 0;
    for (int i = 0; i <= n; i++)
        ans = max(ans, dp[i][i + n - 1]);
    printf("%d\n", ans);
    exit(0);
}

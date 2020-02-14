#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define maxn 2500
#define INF 0x7fffffff

int sum[maxn + 1];
int dp[maxn + 1];

int abs(int a) { return a >= 0 ? a : 0 - a; }

int main() {
    int n, m, a;
    scanf("%d %d", &n, &m);
    //memset(dp, INF, sizeof(dp));
    for (int i = 2; i < maxn + 1; i++)
        dp[i] = INF;
    dp[0] = 0;
    dp[1] = 1;    
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a);
        if (a == 2) a = -1;
        sum[i] = sum[i - 1] + a;
        for (int j = 0; j < i; j++) {
            if ((abs(sum[i] - sum[j]) <= m || abs(sum[i] - sum[j]) == i - j)
                    && dp[j] + 1 < dp[i])
                dp[i] = dp[j] + 1;
        }
    }
    printf("%d", dp[n]);
    return 0;
}

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

#define maxn 200
#define maxm 20
#define infinity 0x7fffffffffffffff

typedef long long ll;

ll dp[maxm][maxn + 1];
ll min(ll a, ll b) { return a < b ? a : b; }
/*int pow(int x, int y) {
    int result = 1;
    for (int i = 0; i < y; i++)
        result *= x;
    return result;
}*/

int main() {
    int n, m, a, b;
    scanf("%d %d", &n, &m);
    scanf("%d %d", &a, &b);
    for (int i = 0; i < m; i++)
        for (int j = 0; j <= n; j++)
            dp[i][j] = infinity;
    for(int i = 0; i <= n; i++)
        dp[0][i] = a * pow(i, b);
    for (int i = 1; i < m; i++) {
        scanf("%d %d", &a, &b);
        for (int j = 0; j <= n; j++)
            for (int k = 0; k <= j; k++)
                dp[i][j] = min(dp[i][j], dp[i - 1][k] + (a * pow(j - k, b)));
    }
    printf("%lld", dp[m - 1][n]);
    return 0;
}

#include "stdio.h"
#include "stdlib.h"

#define maxn 100000

typedef long long ll;

//ll dp[maxn + 1][maxn + 1];
ll dp[2][maxn + 1];

ll max(ll a, ll b) { return a > b ? a : b; }

int main() {
    int n, k, e;
    ll maxsum;
    scanf("%d %d", &n, &k);
    scanf("%d", &e);
    //dp[1][0] = 0;
    //dp[1][1] = maxsum = e;
    dp[0][0] = 0;
    dp[0][1] = maxsum = e;
    int pre = 0, cur = 1;
    for (int i =2; i <= n; i++) {
        //dp[i][0] = maxsum;
        dp[cur][0] = maxsum;
        scanf("%d", &e);
        for (int j = 1; j <= k && j <= i; j++) {
            //dp[i][j] = dp[i - 1][j - 1] + e;
            dp[cur][j] = dp[pre][j - 1] + e;
            //maxsum = max(maxsum, dp[i][j]);
            maxsum = max(maxsum, dp[cur][j]);
        }
        cur = ~cur&1;
        pre = ~pre&1;
    }
    printf("%lld", maxsum);
}

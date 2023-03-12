#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C 4
#define S 100005

int c[C], d[C];
typedef long long ll;
ll dp[C][S];

int main() {
    int i, j, k, n, s;
    ll num;
    for (i = 0; i < C; i++) scanf("%d", &c[i]);
    scanf("%d", &n);
    while (n--) {
        for (i = 0; i < C; i++) scanf("%d", &d[i]);
        scanf("%d", &s);
        memset(dp, 0, sizeof(dp));
        for (i = 0; i <= d[0] && i * c[0] <= s; i++) dp[0][i * c[0]] = 1;
        for (i = 1; i < C; i++)
            for (j = 0; j < c[i]; j++) {
                num = 0ll;
                for (k = 0; k <= (s - j) / c[i]; k++) {
                    num += dp[i - 1][j + k * c[i]];
                    if (k > d[i]) num -= dp[i - 1][j + (k - d[i] - 1) * c[i]];
                    dp[i][j + k * c[i]] = num;
                }
            }
        printf("%lld\n", dp[C - 1][s]);
    }
    exit(0);
}

#include "stdio.h"
#include "stdlib.h"

#define maxn 16
typedef long long LL;

LL dp[maxn][1 << maxn];
LL s[maxn];

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++)
        scanf("%lld", s + i);
    for (int i = 0; i < n; i++)
        dp[i][1 << (n - i - 1)] = 1;
    int new_state;
    for (int state = 1; state < (1 << n); state++)
        for (int i = 0; i < n; i++) {
            if (state & (1 << (n - i - 1))) continue;
            new_state = state | (1 << (n - i - 1));
            for (int j = 0; j < n; j++) {
                if (!(state & (1 << (n - j - 1)))) continue;
                if (abs(s[i] - s[j]) <= k) continue;
                dp[i][new_state] += dp[j][state];
            }
        }
    LL result = 0;
    for (int i = 0; i < n; i++)
        result += dp[i][(1 << n) - 1];
    printf ("%lld", result);
    return 0;
}

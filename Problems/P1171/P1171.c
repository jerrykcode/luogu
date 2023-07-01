#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20

int s[N][N];
int dp[1<<N][N];

static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, i, j, state, pre_state, ans;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &s[i][j]);
    memset(dp, 0x7f, sizeof(dp));
    dp[1][0] = 0;
    for (state = 0b11; state < 1 << n; state++) {
        if (!(state & 1)) continue;
        for (i = 1; i < n; i++)
            if (state & (1 << i)) {
                dp[state][i] = 0x7f7f7f7f;
                pre_state = state & (~(1 << i));
                for (j = 0; j < n; j++)
                    if (pre_state & (1 << j))
                        dp[state][i] = min(dp[state][i], dp[pre_state][j] + s[j][i]);
            }
    }
    ans = 0x7f7f7f7f;
    for (i = 0; i < n; i++)
        ans = min(ans, dp[(1 << n) - 1][i] + s[i][0]);
    printf("%d", ans);
    exit(0);
}

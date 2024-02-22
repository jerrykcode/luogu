#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define C 100005
int dp[C][2];
int main() {
	int n, i, j, c, sum, pre, cur;
    memset(dp, 0xff, sizeof(dp));
    scanf("%d %d", &n, &c);
    sum = c;
    dp[0][0] = 0;
    dp[c][0] = c;
    cur = 1, pre = 0;
    for (i = 1; i < n; i++) {
        scanf("%d", &c);
        sum += c;
        for (j = 0; j <= sum; j++) {
            if (dp[j][pre] >= 0 && dp[j][pre] > dp[j][cur]) dp[j][cur] = dp[j][pre];
            if (j + c <= sum && dp[j + c][pre] >= 0 && dp[j + c][pre] + c > dp[j][cur]) dp[j][cur] = dp[j + c][pre] + c;
            if (dp[abs(j - c)][pre] >= 0 && dp[abs(j - c)][pre] + c > dp[j][cur]) dp[j][cur] = dp[abs(j - c)][pre] + c;
        }
        cur += pre;
        pre = cur - pre;
        cur -= pre;
    }
    printf("%d", sum - (dp[0][pre] / 2));
	exit(0);
}

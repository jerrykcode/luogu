#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define maxn 10000
#define maxk 100

bool dp[2][maxk];

int mod(int a, int k) {
    int res = a % k;
    if (res < 0) res += k;
    return res;
}

int main() {
    int t, n, k, a, cur = 0, pre = 1, tmp;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %d", &n, &k);
        scanf("%d", &a);
        memset(dp, false, sizeof(dp));
        dp[cur][mod(a, k)] = true;
        for (int i = 1; i < n; i++) {
            scanf("%d", &a);
            tmp = cur;
            cur = pre;
            pre = tmp;
            for (int j = 0; j < k; j++) {
                dp[cur][j] = dp[pre][mod(j - a, k)] || dp[pre][mod(j + a, k)];
            }
        }
        if (dp[cur][0]) printf("Divisible\n");
        else printf("Not divisible\n");
    }
    exit(0);
}

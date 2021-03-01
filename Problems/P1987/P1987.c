#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxn 1000

int dp[2][maxn];

typedef struct MoneyT {
    int init;
    int desc;
} *PMoneyT;

int compare(const void *a, const void *b) {
    return ((PMoneyT)b)->desc - ((PMoneyT)a)->desc;
}

struct MoneyT money[maxn];

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, k, i, j, pre, cur, tmp, ans;
    while (1) {
        scanf("%d %d", &n, &k);
        if (n == 0) break;
        for (i = 0; i < n; i++)
            scanf("%d", &money[i].init);
        for (i = 0; i < n; i++)
            scanf("%d", &money[i].desc);
        qsort(money, n, sizeof(struct MoneyT), compare);
        memset(dp, 0, sizeof(dp));
        pre = 0;
        cur = 1;
        for (i = 1; i <= n; i++) {            
            for (j = 1; j <= min(i, k); j++) {
                dp[cur][j] = max(dp[pre][j], 
                    dp[pre][j - 1] + 
                    max(0, money[i - 1].init - (j - 1) * money[i - 1].desc));
            }
            tmp = cur;
            cur = pre;
            pre = tmp;
        }
        ans = 0;
        for (i = 1; i <= k; i++)
            ans = max(ans, dp[pre][i]);
        printf("%d\n", ans);
    }
    exit(0);
}

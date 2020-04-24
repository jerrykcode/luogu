#include "stdio.h"
#include "stdlib.h"

#define maxn 2000
#define maxh 2000

short num[maxn + 1][maxh + 1];
int dp[maxn + 1][maxh + 1];
int ans[maxh + 1];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, h, delta, ni, tij;
    scanf("%d %d %d", &n, &h, &delta);
    for (int tree = 1; tree <= n; tree++) {
        scanf("%d", &ni);
        for (int i = 0; i < ni; i++) {
            scanf("%d", &tij);
            num[tree][tij]++;
        }
    }
    for (int tree = 1; tree <= n; tree++) {
        dp[tree][0] = num[tree][0];
        ans[0] = max(ans[0], dp[tree][0]);
    }
    int m;
    for (int height = 1; height <= h; height++)
        for (int tree = 1; tree <= n; tree++) {
            dp[tree][height] = num[tree][height] 
                + max(dp[tree][height - 1], 
                      (height >= delta ? ans[height - delta] : 0));
            ans[height] = max(ans[height], dp[tree][height]);
        }
    printf("%d", ans[h]);
    return 0;
}

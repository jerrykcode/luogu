#include "stdio.h"
#include "stdlib.h"

int dp[101][101]; //dp[j][k]表示花费j块大洋, k的人品能得到的mm数量
int dp_t[101][101]; //dp_t[j][k]表示花费j块大洋, k的人品能得到最多mm时所用的最少时间
int rmb[101], rp[101], t[101];

int main() {
    int n, m, r;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d %d %d", rmb + i, rp + i, t + i);
    }
    scanf("%d %d", &m, &r);
    int tmp_dp, tmp_dp_t;
    for (int i = 1; i <= n; i++) {
        //二维01背包
        for (int j = m; j >= rmb[i]; j--) {
            for (int k = r; k >= rp[i]; k--) {                
                tmp_dp = dp[j - rmb[i]][k - rp[i]] + 1; //选择第i个mm
                tmp_dp_t = dp_t[j - rmb[i]][k - rp[i]] + t[i];
                if (tmp_dp > dp[j][k]) { 
                    dp[j][k] = tmp_dp;
                    dp_t[j][k] = tmp_dp_t;
                }
                else if (tmp_dp == dp[j][k] && tmp_dp_t < dp_t[j][k]) {
                    dp_t[j][k] = tmp_dp_t;
                }
                if (i == n) break;
            }
        }
    }
    printf("%d", dp_t[m][r]);
    return 0;
}

#include "stdio.h"
#include "stdlib.h"

#define maxn 248

int dp[maxn][maxn];
//dp[l][r]表示将第l个数到第r个数合并能得到的最大数字, 若无法合并则为0
//对于所有的i, dp[i][i] = 第i个数的值
//统计所有dp[l][r]中的最大值作为答案

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n;
    scanf("%d", &n);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &dp[i][i]);
        ans = max(ans, dp[i][i]);
    }
    for (int len = 2; len <= n; len++) //枚举长度len
        for (int l = 0, r = l + len - 1; r < n; l++, r++) { //枚举区间[l, r]
            for (int k = l; k < r; k++) //枚举k, 尝试区间[l, r]由[l, k]和[k + 1, r]合并而成的情况
                if (dp[l][k] /*&& dp[k + 1][r]*/ && dp[l][k] == dp[k + 1][r]) //不为0且相同则可以合并
                    dp[l][r] = max(dp[l][r], dp[l][k] + 1); //转移方程, dp[l][k]和dp[k + 1][r]的值在之前已经被计算出来
            ans = max(ans, dp[l][r]); //统计答案
        }
    printf("%d", ans);
    return 0;
}

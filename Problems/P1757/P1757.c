#include "stdio.h"
#include "stdlib.h"

int dp[1001]; //dp[i]表示容量i的背包可以装到的最大价值
int table[101][100]; //table[i][j]表示第i组第j个的编号
int top[101]; //top[i]表示第i组的最后一个物品的编号
int weight[1001]; //weight[i]表示编号i的物品的重量
int value[1001]; //value[i]表示编号i的物品的价值

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int main() {
    int m, n, t, max_t = 0, idx;
    scanf("%d %d", &m, &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d %d %d", &weight[i], &value[i], &t);
        table[t][++top[t]] = i;
        if (t > max_t) max_t = t; //记录最大组编号
    }
    for (int i = 1; i <= max_t; i++) { //遍历每组
        for (int j = m; j > 0; j--) { //遍历容量，一位数组，所以要倒序
            for (int k = 1; k <= top[i]; k++) { //遍历第i组的每一个物品
                idx = table[i][k]; //第i组第k个物品的编号
                if (j >= weight[idx])
                    dp[j] = max(dp[j], dp[j - weight[idx]] + value[idx]); //可以找出第i组中最优的物品并决定是否选择
            }
            if (i == max_t) break;
        }
    }
    printf("%d", dp[m]);
    return 0;
}

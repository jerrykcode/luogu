#include "stdio.h"
#include "stdlib.h"

#define maxm 12
#define maxnum 0x1000 /* 1 << 12 */
#define mod 100000000

int status[maxnum], top;
int row[maxm];
int dp[maxm][maxnum];

//初始化
void init(int n) {
    int p = 1 << n; //最多可能的种植状态数量
    top = 0;
    for (int i = 0; i < p; i++) { //遍历每种状态
        //若没有相邻种植, 则记录i
        if (!(i & (i << 1))) status[top++] = i;
    }
}

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    init(n);
    int flag;
    for (int i = 0; i < m; i++) {        
        for (int j = 0; j < n; j++) {
            scanf("%d", &flag); //输入, 1表示可以种植, 0表示不可以
            //row[i]的二进制位, 1表示不可种植, 0表示可以
            //初始时row[i]=0, 所有二进制位为0
            if (!flag) row[i] += (1 << (n - j - 1)); //输入为0, 则row[i]相应的二进制位更新为1
        }
    }
    //对于第0行特殊处理
    for (int i = 0; i < top; i++) {        
        if (!(status[i] & row[0])) dp[0][i] = 1; //若status[i]适用于第0行
    }
    for (int i = 1; i < m; i++) { //遍历1至m-1行
        for (int j = 0; j < top; j++) {
            if (status[j] & row[i]) continue; //status[j]不适用于第i行, 则continue
            //找到一个适用于第i行的status[j]
            for (int k = 0; k < top; k++) {
                if (status[k] & row[i - 1]) continue; //status[k]不适用于第i - 1行, 则continue
                //找到一个适用于第i - 1行的status[k]
                if (status[j] & status[k]) continue; //若status[j]与status[k]不兼容, 则continue
                //到这里第i行选择的第j种状态和第i - 1行选择的第k种状态是兼容的
                dp[i][j] += dp[i - 1][k];
                dp[i][j] %= mod;
            }
        }
    }
    int res = 0;
    for (int i = 0; i < top; i++) {
        res += dp[m - 1][i];
        res %= mod;
    }
    printf("%d", res);
    return 0;
}

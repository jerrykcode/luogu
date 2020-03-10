//状态压缩模板题

#include "stdio.h"
#include "stdlib.h"

#define maxn 100
#define maxm 10
#define maxs 1024

typedef unsigned char byte;

short states[maxs];
int top = 0;
byte num[maxs];
short row[maxn];
short dp[2][maxs][maxs];

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (short i = 0; i < (1 << m); i++) { //枚举一行所有可能的状态
        if (!((i << 1) & i) && !((i << 2) & i)) states[top++] = i; //横向左右各两格不会冲突
        for (int j = 0; j < m; j++)
            if ((1 << (m - j - 1)) & i) num[i]++; //记录i状态中1的个数
    }
    char ch;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            do { scanf("%c", &ch); } while (ch != 'H' && ch != 'P'); //忽略其他字符
            if (ch == 'H') row[i] |= (1 << (m - j - 1));
        }
    }
    if (n >= 2) {
        //对前2行特殊处理
        for (int i = 0; i < top; i++) {
            if (states[i] & row[1]) continue;
            for (int j = 0; j < top; j++) {
                if (states[j] & row[0]) continue;
                if (states[i] & states[j]) continue;
                dp[1][i][j] = num[states[i]] + num[states[j]];
            }
        }
    }
    int cur = 0, pre = 1, tmp; //cur表示当前行, pre为上一行, dp数组表示行的这一维被压缩
    for (int r = 2; r < n; r++) { //遍历行
        for (int i = 0; i < top; i++) { //为第r行枚举状态
            if (states[i] & row[r]) continue;
            //到这里, 找到了符合第r行的状态, 该状态为states[i]
            for (int j = 0; j < top; j++) { //为第r-1行枚举状态
                if (states[j] & row[r - 1]) continue;
                //找到符合第r-1行的状态
                if (states[i] & states[j]) continue;
                //找到与states[i]兼容的状态, 为states[j]
                short max_jk = 0;
                for (int k = 0; k < top; k++) { //为第r-2行枚举状态
                    if (states[k] & row[r - 2]) continue;                    
                    if (states[i] & states[k]) continue;
                    if (states[j] & states[k]) continue;
                    //至此, 找到符合第r-2行, 且与states[i], states[j]都兼容的状态, states[k]
                    if (dp[pre][j][k] > max_jk) max_jk = dp[pre][j][k]; //在i, j确定时, 在所有符合要求的k中, 找到最大的dp[pre][j][k]
                }
                if (max_jk > 0) { //符合要求的k存在
                    dp[cur][i][j] = num[states[i]] + max_jk;
                }
            }
        }
        //cur, pre交换数值
        tmp = pre;
        pre = cur;
        cur = tmp;
    }
    short result = 0;
    for (int i = 0; i < top; i++)
        for (int j = 0; j < top; j++) {
            if (dp[pre][i][j] > result) result = dp[pre][i][j]; //注意最后一次遍历结束后, 最后一行用pre表示
        }
    printf("%d", result);
    return 0;
}

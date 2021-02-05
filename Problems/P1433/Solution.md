# P1433 吃奶酪

[题目传送门](https://www.luogu.com.cn/problem/P1433)

## 题目描述

房间里放着n块奶酪。一只小老鼠要把它们都吃掉，问至少要跑多少距离？老鼠一开始在 (0,0) 点处。

## 输入格式
第一行有一个整数，表示奶酪的数量n

第2行到第(n + 1)行，每行两个实数，第(i + 1)行的实数分别表示第i块奶酪的横纵座标xi, yi

## 输出格式
输出一行一个实数，表示要跑的最少距离，保留 2 位小数。

## 输入输出样例

### 输入 #1
4
1 1
1 -1
-1 1
-1 -1
### 输出 #1
7.41

## 思路

**状压DP**，15位二进制数表示一个状态(第i位为1表示第i个吃了，0表示没吃)，  
dp[state][i]表示到达状态state并且最后吃的是第i个,  
枚举状态，对每个状态state, 枚举其二进制位，若第i位为1，则dp[state][i]可以从state第i位改为0的那个状态转移而来，假设那个状态为pre,则  
dp[state][i] = min{dp[pre][j] + 第i个和第j个之间的距离}, 其中j为pre中所有为1的位  
由于状态从小到大枚举，故计算dp[state][i]时 dp[pre][j]已经被计算过，因为pre一定小于state

## 代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define maxn 15
#define INF (0x7fffffff*1.0)

double dp[1<<maxn][maxn];
double x[maxn], y[maxn];

double square(double a) {
    return a*a;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt(square(x1 - x2) + square(y1 - y2));
}

double dist_index(int i, int j) {
    return dist(x[i], y[i], x[j], y[j]);
}

double min(double a, double b) {
    return a < b ? a : b;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &x[i], &y[i]);
        dp[1 << i][i] = dist(x[i], y[i], 0, 0); //Init
    }
    int state, pre, count;
    for (state = 3; state < 1 << n; state++) {
        count = 0;
        for (int i = 0; i < n; i++)
            if (state & (1 << i)) 
                count++;
        if (count <= 1) continue; //二进制位只有一个1的 在初始化时已经计算过
        for (int i = 0; i < n; i++) {
            if (state & (1 << i)) {
                dp[state][i] = INF; //Init
                pre = state ^ (1 << i); //state可以从pre转来
                for (int j = 0; j < n; j++)
                    if (pre & (1 << j)) {
                        dp[state][i] = min(
                            dp[state][i],
                            dp[pre][j] + dist_index(i, j)
                        );
                    }
            }
        }
    }
    double ans = INF;
    for (int i = 0; i < n; i++) {
        ans = min(ans, dp[(1 << n) - 1][i]);
    }
    printf("%0.2lf\n", ans);
    exit(0);
}
```

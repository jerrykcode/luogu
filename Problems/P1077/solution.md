# P1077 [NOIP2012 普及组] 摆花
[题目传送门](https://www.luogu.com.cn/problem/P1077)

## 题目描述

小明的花店新开张，为了吸引顾客，他想在花店的门口摆上一排花，共m盆。通过调查顾客的喜好，小明列出了顾客最喜欢的n种花，
从1到n标号。为了在门口展出更多种花，规定第i种花不能超过ai盆，摆花时同一种花放在一起，且不同种类的花需按标号的从大到小的顺序依次摆列。

试编程计算，一共有多少种不同的摆花方案。

## 输入格式

第一行包含两个正整数n和m，中间用一个空格隔开。  
第二行有n个整数，每两个整数之间用一个空格隔开，依次表示a1, a2, ..., an。

## 输出格式

一个整数，表示有多少种方案。注意：因为方案数可能很多，请输出方案数对1000007取模的结果。

## 输入输出样例

### 输入 #1
2 4  
3 2  

### 输出 #1
2

**注** 一些种类的花可以一个也不摆。

## 思路

dp[i][j]表示前i种花，摆放到1~j位置的方案数，  
最终答案dp[n][m]  
初始化dp[i][0] = 1, 即摆放0个位置只有1种方案，即什么也不摆  
转移方程 dp[i][j] = sum{dp[i - 1][k]}, min(j - a[i], 0) <= k <= j  
时间复杂度 O(n\*m\*a)

一些优化:  

(1) dp[i][j]只从dp[i - 1][k]转移而来，所以i这一维可以压缩掉，dp[cur][j]从dp[pre][k]处转移即可。  
每次枚举完j后交换cur, pre的值。  

(2) 对于dp[cur][j]和dp[cur][j - 1]，  
dp[cur][j]从dp[pre][k]转移，k的范围: [min(j - a[i], 0), j]  
dp[cur][j - 1]从dp[pre][k]转移，k的范围: [min(j - a[i] - 1, 0), j - 1]  
可以使用窗口记录一段连续的dp[pre][k]之和，每次j++后，窗口+=dp[pre][j], 同时若j - a[i] - 1 >= 0则 窗口-=dp[pre][j - a[i] - 1]  
即通过滑动窗口代替枚举k的值，时间复杂度将为O(n\*m)  

## 代码

```c
#include <stdio.h>
#include <stdlib.h>

#define MAXN 100
#define MAXM 100

int dp[2][MAXM + 1];

#define MOD 1000007

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int cur = 0, pre = 1, a, window;
    dp[cur][0] = dp[pre][0] = 1;
    for (int i = 0; i < n; i++) {
        scanf("%d", &a);
        window = dp[pre][0];
        for (int j = 1; j <= m; j++) {
            window += dp[pre][j];
            if (j - a - 1 >= 0) window -= dp[pre][j - a - 1];
            dp[cur][j] = window % MOD;
        }
        cur += pre;
        pre = cur - pre;
        cur -= pre;
    }
    printf("%d\n", dp[pre][m]);
    exit(0);
}
```

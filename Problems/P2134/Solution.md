# P2134 百日旅行
[题目传送门](https://www.luogu.com.cn/problem/P2134)

## 题目背景

> 重要的不是去哪里，而是和你在一起。——小红

对小明和小红来说，2014年7月29日是一个美好的日子。这一天是他们相识100天的纪念日。

（小明：小红，感谢你2场大考时默默的支持，100个日夜的陪伴；感谢你照亮我100个美好的日子，给我留下无数美好的回忆……在这个美好的日子里，我准备带你去旅行。）

## 题目描述

小明和小红还剩下 n 天的假期，小明可以安排旅行的计划。如果连续 x 天旅游，小明需要花旅行费用 p\*x\*x 元；如果连续 x 天不旅游，小明需要请小红吃饭，花费为 q\*x 元（ p,q 都是输入的常数）。

请你帮小明写一个程序，计算出假期里他至少需要花费多少元。

## 输入格式

输入只有一行，包含三个空格隔开的正整数 n,p,q。

## 输出格式

一行，一个正整数表示小明至少需要花费多少元。

## 输入输出样例

### 输入 #1
6 1 7

### 输出 #1
20

## 说明/提示

### 数据规模与约定

* 对于 20% 数据，1 <= n <= 20
* 对于 90% 数据，1 <= n <= 1000, 1 <= p <= 2000, 1 <= q <= 10000
对于 100\%100% 数据，1 <= n <= 2\*10^5, 1 <= q <= p <= 10^4

## 思路

动态规划

dp[i][0]表示前i天，最后一天(第i天)吃饭的最优解，dp[i][1]表示前i天(第i天)最后一天旅游的最优解，  
初始化: dp[0][0] = dp[0][1] = 0  
转移方程:  
dp[i][0] = min(dp[i - 1][0], dp[i - 1][1]) + q  
dp[i][1] = min(dp[j][0] + (i - j)\*(i - j)\*p), 0 <= j < i  
结果:  
min(dp[n][0], dp[n][1])  

转移方程复杂度O(n^2), 可以使用斜率优化，见代码开头注释:

## Code

```c
/*
dp[i][0] = min{dp[i - 1][0], dp[i - 1][1]} + q
dp[i][1] = min{dp[j][0] + (i - j)*(i - j)*p}, 0 <= j < i

斜率优化dp[i][1]:
对于dp[i][1], 求最优的j, 假设 0 <= j < k < i, 且 k优于j, 则:
    dp[k][0] + (i - k)*(i - k)*p < dp[j][0] + (i - j)*(i - j)*p,
令f[x] = dp[x][0], 即:
    f[k] + (i - k)*(i - k)*p < f[j] + (i - j)*(i - j)*p
=>  f[k] + i*i*p - 2*i*k*p + k*k*p < f[j] + i*i*p - 2*i*j*p + j*j*p
=>  f[k] - 2*i*p*k + k*k*p < f[j] - 2*i*p*j + j*j*p
=>  f[k] + k*k*p - (f[j] + j*j*p) < 2*i*p*(k - j)
由于k - j > 0,

    (f[k] + k*k*p - (f[j] + j*j*p)) / (k - j) < 2*i*p

j对应二维点(j, f[j] + j*j*p), k对应二维点(k, f[k] + k*k*p),
连接j, k的直线斜率 < 2*i*p <=> k优于j
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
long long dp[MAXN][2];
int queue[MAXN];
int head, tail;
long long p;

double slope(long long j, long long k) {
    return ((dp[k][0] + k*k*p - dp[j][0] - j*j*p)*1.0) / (k - j);
}
long long min(long long a, long long b) { return a < b ? a : b; }
int main() {
    int n, q;
    long long i, j;
    scanf("%d %lld %d", &n, &p, &q);
    head = 0, tail = -1;
    queue[head] = 0;
    for (i = 1; i <= n; i++) {
        dp[i][0] = min(dp[i - 1][0], dp[i - 1][1]) + q;
        dp[i][1] = i*i*p; //初始化
        double k = 2.0*i*p;
        while (head < tail && slope(queue[head], queue[head + 1]) < k) head++;
        j = queue[head];
        dp[i][1] = min(dp[i][1], dp[j][0] + (i - j)*(i - j)*p);
        while (head < tail && slope(queue[tail - 1], queue[tail]) > slope(queue[tail], i)) tail--;
        queue[++tail] = i;
    }
    printf("%lld\n", min(dp[n][0], dp[n][1]));
    exit(0);
}
```

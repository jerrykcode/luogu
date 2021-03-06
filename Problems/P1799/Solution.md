# P1799 数列

[原题地址](https://www.luogu.com.cn/problem/P1799)

## 题目描述

虽然msh长大了，但她还是很喜欢找点游戏自娱自乐。有一天，她在纸上写了一串数字：1，1，2，5，4。接着她擦掉了一个1，结果发现剩下1，2，4都在自己所在的位置上，即1在第1位，2在第2位，4在第4位。她希望擦掉某些数后，剩下的数列中在自己位置上的数尽量多。她发现这个游戏很好玩，于是开始乐此不疲地玩起来……不过她不能确定最多能有多少个数在自己的位置上，所以找到你，请你帮忙计算一下！

## 输入格式

第一行为一个数n，表示数列的长度。

接下来一行为n个用空格隔开的正整数，第i行表示数Ai。

## 输出格式

一行一个整数，表示擦掉某些数后，最后剩下的数列中最多能有多少个数在自己的位置上，即Ai=i最多能有多少。

## 输入输出样例

### 输入 #1

> 5  
> 1 1 2 5 4

### 输出 #1

> 3

## 说明/提示

对于20%的数据，n≤20;

对于60%的数据，n≤100;

对于100%的数据，n≤l000。



## 思路

### [参考](https://siyuan.blog.luogu.org/solution-p1799)

dp[i][j]表示前i个数(从1开始计数,包括第i个)中保留j个数(0 <= j <= i)时的最优解,  
状态转移方程: `dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - 1] + (a[i] == j ? 1 : 0))`  
对1 <= i <= n, 考虑a[i]\(第i个数\)是否留下, 若不留则前i-1个数中保留j个即dp[i - 1][j],   
若保留a[i]则前面i-1个数中保留j-1个, 即dp[i - 1][j - 1],  
而如果a[i]在自己的位置上, 则保留a[i]可以为dp[i][j]贡献1,  
由于前i个数中保留了j个数, 所以a[i]的位置为j, 那么 "若a[i]等于j, 就说明a[i]在自己的位置上"  

```c
dp[i][j] = max(dp[i - 1][j] /* 不保留a[i] */,
               dp[i - 1][j - 1] + (a[i] == j ? 1 : 0) /* 保留a[i] */)
```

可以将dp优化成1维数组,  
a[i]边输入边处理, 数组a也不需要了.  

## 代码

### C

```c
#include "stdio.h"
#include "stdlib.h"

int dp[1001];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n, a, res = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a);
        for (int j = i; j >= 1; j--) {
            dp[j] = max(dp[j], dp[j - 1] + (a == j ? 1 : 0));
            if (dp[j] > res) res = dp[j];
        }
    }
    printf("%d", res);
    return 0;
}
```

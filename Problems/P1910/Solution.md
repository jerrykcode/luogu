# P1910 L国的战斗之间谍

[原题地址](https://www.luogu.com.cn/problem/P1910)

## 题目背景

L国即将与I国发动战争！！

## 题目描述

俗话说的好：“知己知彼，百战不殆”。L国的指挥官想派出间谍前往I国，于是，选人工作就落到了你身上。  
你现在有N个人选，每个人都有这样一些数据：A（能得到多少资料）、B（伪装能力有多差）、C（要多少工资）。已知敌人的探查间谍能力为M（即去的所有人B的和要小于等于M）和手头有X元钱，请问能拿到多少资料？

## 输入格式

> N M X  
> A1 B1 C1  
> A2 B2 C2  
> ………………  
> AN BN CN  

## 输出格式

能得到的资料总数


## 输入输出样例

### 输入 #1

> 3 10 12  
> 10 1 11  
> 1 9 1  
> 7 10 12  

### 输出 #1
> 11


## 说明/提示

### 数据范围：

1≤n≤100,1≤m≤1000, 1≤x≤1000


## 思路

## 二维背包，01背包稍微延伸一点点

背包有两个容量(把其中一个容量看成背包可以承受的最大重量更好理解)，  

题目转化为:  
背包有一个最大容量，和一个可以承受的最大重量，N个物品，每个给出其体积和重量以及价值，怎么装使得总价值最大?  

dp[j][k]表示容量为j，重量不超过k的背包可以装到的最大价值，  
枚举每个物品，嵌套循环容量j和重量k，决定这个物品选还是不选就可以啦。  

代码中把每个物品的容量和重量用二维数组存储。


## 代码

```c
#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int value[100];
int weight[2][100];
int dp[1001][1001];

int main() {
    int n, m, x;
    scanf("%d %d %d", &n, &m, &x);
    for (int i = 0; i < n; i++)
        scanf("%d %d %d", value + i, weight[0] + i, weight[1] + i);
    for (int i = 0; i < n; i++)
        for (int j = m; j >= weight[0][i]; j--)
            for (int k = x; k >= weight[1][i]; k--) {
                dp[j][k] = max(dp[j][k], dp[j - weight[0][i]][k - weight[1][i]] + value[i]);
                if (i == n - 1) break;
            }
    printf("%d", dp[m][x]);
    return 0;
}
```

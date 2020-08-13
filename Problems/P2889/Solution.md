# P2889  [USACO07NOV]Milking Time S

[原题地址](https://www.luogu.com.cn/problem/P2889)

## 题目描述

Bessie 可以在接下来N个小时内产奶，为了方便，我们把这N个小时 0 ~ N - 1 编号。  
FJ 在这N个小时内有M段时间可以来给 Bessie 挤奶，第i段时间从Starti开始到Endi结束，可以得到Effi加仑牛奶。  
每次 FJ 给 Bessie 挤奶之后，Bessie 都要休息R个小时，FJ 才能开始下一次挤奶。  
现在，FJ 需要您计算出 Bessie 在这N个小时内最多产多少奶。  
## 输入格式

第一行有三个整数，分别表示 N,M,R   

第 2 ~ M + 1 行，第 i+1 行有三个整数 Starti,Endi,Effi, 描述一段挤奶的时间。  
## 输出格式

输出一行一个整数表示答案。  
## 输入输出样例
### 输入 #1

> 12 4 2  
> 1 2 8  
> 10 12 19  
> 3 6 24  
> 7 10 31  

### 输出 #1

> 43  

## 说明/提示
数据规模与约定

对于全部的测试点，保证 1 ≤ N ≤ 10^6, 1 ≤ M ≤ 10^3，1 ≤ Starti < endi ≤ N，1 ≤ Effi ≤ 10^6  


## 思路

对所有时间段按end从小到大排序  
dp[i]表示到第i个时间段结束时的最优解  
dp[0] = 第0个时间段的eff  
dp[i]由两个状态转移过来:  
* 不选第i个时间段，保持上个时间段结束时的最优解，即dp[i - 1]  
* 选择第i个时间段，二分计算出不受到第i个时间段影响的最晚end时间段k，dp[i] = dp[k] + 第i个时间段的eff  

## 代码

```c
#include <stdio.h>
#include <stdlib.h>

#define maxm 1000

typedef struct interval {
    int start, end;
    int eff;
} interval;

int compare(const void *a, const void *b) {
    return ((interval *)a)->end - ((interval *)b)->end;
}

interval ins[maxm];
int dp[maxm];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n, m, r;
    scanf("%d %d %d", &n, &m, &r);
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &ins[i].start, &ins[i].end, &ins[i].eff);
    }
    qsort(ins, m, sizeof(interval), compare);
    dp[0] = ins[0].eff;
    int left, right, mid, k;
    for (int i = 1; i < m; i++) {
        //binary search
        left = 0, right = i;
        k = -1;
        while (left <= right) {
            mid = (left + right) >> 1;
            if (ins[mid].end <= ins[i].start - r) {
                k = mid;
                left = mid + 1;
            }
            else right = mid - 1;
        }        
        //DP
        dp[i] = max(dp[i - 1], (k != -1 ? dp[k] : 0) + ins[i].eff);
    }
    printf("%d\n", dp[m - 1]);
    exit(0);
}
```

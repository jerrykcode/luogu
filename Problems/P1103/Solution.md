# P1103 书本整理
[原题地址](https://www.luogu.com.cn/problem/P1103)  

## 题目描述

Frank是一个非常喜爱整洁的人。他有一大堆书和一个书架，想要把书放在书架上。书架可以放下所有的书，所以Frank首先将书按高度顺序排列在书架上。但是Frank发现，由于很多书的宽度不同，所以书看起来还是非常不整齐。于是他决定从中拿掉k本书，使得书架可以看起来整齐一点。

书架的不整齐度是这样定义的：每两本书宽度的差的绝对值的和。例如有4本书：

1×2  
5×3  
2×4  
3×1  
那么Frank将其排列整齐后是：

1×2  
2×4  
3×1  
5×3  
不整齐度就是 2 + 3 + 2 = 7  

已知每本书的高度都不一样，请你求出去掉k本书后的最小的不整齐度。

## 输入格式

第一行两个数字n和k，代表书有几本，从中去掉几本。(1 ≤ n ≤ 100, 1 ≤ k < n)  

下面的n行，每行两个数字表示一本书的高度和宽度，均小于200。

保证高度不重复  

## 输出格式

一行一个整数，表示书架的最小不整齐度。  
## 输入输出样例

### 输入 #1
> 4 1  
> 1 2  
> 2 4  
> 3 1  
> 5 3  

### 输出 #1
> 3  

## 思路

首先对书按高度排序, 然后:  
dp[i][j][l]表示前i本书中删掉j本, 并且以第l本书作为剩余书中`结尾`的一本时最小的不整齐度  
初始化: dp[1][0][1] = 0  
        对于任何一个i, dp[i][i][l]即删掉所有书的情况不需要考虑,   
        而dp[i][i - 1][l]即删掉i - 1本书, 只剩下一本书的情况, 初始化为0  
        其他dp[i][j][l]初始化为一个很大的值  
状态转移: 对于每一个i, 枚举j从0到min(k, i - 2), 即删掉书的数量不能超过k, 也不能删太多以至于剩下的书少于2本  
          对于每一对i, j,  
          (1) 若保留第i本书不删除, 那么计算dp[i][j][i]的值(前i本书删掉j本, 以第i本书结尾):  
          前i - 1本书中需要删掉j本, 假设前i - 1本书中删掉j本后剩下的书以第l本结尾,  
          那么dp[i][j][i] = min(dp[i][j][i], dp[i - 1][j][l] + (第i本和第l本书构成的不整齐度));  
          枚举l, 就可以计算出dp[i][j][i].  
          注意l的枚举范围:  
          由于前i - 1本书里删掉了j本, 那么前i - 1本书中剩下了i - 1 - j本, 那么这i - 1 - j本书中结尾的书必定大于等于i - 1 - j, 小于等于i - 1  

  (2) 再考虑删除第i本书的情况. 前i - 1本书中删了j - 1本,  
          而且由于第i本书被删了, 所以剩下的书不可能以i结尾, 而是前i - 1本书剩下的书中以哪一本结尾, 前i本书中剩下的就以哪一本结尾,  
          即枚举l, dp[i][j][l] = dp[i - 1][j - 1][l]  
          注意l的枚举范围:  
          由于前i - 1本书里删了j - 1本, 那么前i - 1本书中剩下了i - j本, 那么这i - 1本书中结尾的书必定大于等于i - j, 小于等于i - 1  
          
统计答案: 枚举l(n - k <= l <= n), 计算dp[n][k][l]中的最大值

## 代码

```c
#include "stdio.h"
#include "stdlib.h"

#define maxn 100
#define maxk 99
#define inf  0x3fffffff    //注意inf不能设置为0xffffffff, 因为状态转移时有可能把inf加上一个值, 要防止溢出

typedef unsigned char byte;   //书的长, 宽均小于200, unsigned char可以表达[0, 255]

struct Book {
    byte height, width;
} books[maxn + 1];
int compare(const void * a, const void * b) {
    return ((struct Book*)a)->height - ((struct Book*)b)->height;
}

int dp[maxn + 1][maxk + 1][maxn + 1];

int min(int a, int b) { return a < b ? a : b; }
int abs(int a) { return a >= 0 ? a : 0 - a; }

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%hhu %hhu", &books[i].height, &books[i].width);
    }
    qsort(books + 1, n, sizeof(struct Book), compare);
    //初始化
    dp[1][0][1] = 0;
    for (int i = 2; i <= n; i++) {
        for (int j = 0; j <= min(k, i - 2); j++)
            for (int l = i - j; l <= i; l++) dp[i][j][l] = inf;
        /*for (int l = 1; l <= i; l++)
            dp[i][i - 1][l] = 0; //删到只剩一本书, 不整齐度为0, 但是由于dp数组本来就是全0, 所以这里的赋值就不必要了
        */
    }
    //状态转移
    for (int i = 2; i <= n; i++)
        for (int j = 0; j <= min(k, i - 2); j++) { //枚举j
            //保留第i本, 计算dp[i][j][i]
            for (int l = i - 1 - j; l < i; l++) //枚举l
                dp[i][j][i] = min(dp[i][j][i], dp[i - 1][j][l] + abs(books[i].width - books[l].width));
            //不保留第i本
            for (int l = i - j; l < i; l++) //枚举l
                if (j > 0) dp[i][j][l] = dp[i - 1][j - 1][l];
        }
    int ans = inf;
    for (int l = n - k; l <= n; l++)
        ans = min(ans, dp[n][k][l]); //统计答案
    printf("%d", ans);
    return 0;
}
```


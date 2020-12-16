# P1005 矩阵取数游戏
[题目传送门](https://www.luogu.com.cn/problem/P1005)


## 题目描述

帅帅经常跟同学玩一个矩阵取数游戏：对于一个给定的 n×m 的矩阵，矩阵中的每个元素 a{i,j} 均为非负整数。游戏规则如下:  
  
每次取数时须从每行各取走一个元素，共 n 个。经过 m 次后取完矩阵内所有元素;  
每次取走的各个元素只能是该元素所在行的行首或行尾;  
每次取数都有一个得分值，为每行取数的得分之和，每行取数的得分 = 被取走的元素值×2的i次方，其中 i 表示第 i 次取数(从 1 开始编号);
游戏结束总得分为 m 次取数得分之和.  
帅帅想请你帮忙写一个程序，对于任意矩阵，可以求出取数后的最大得分.  

## 输入格式

输入文件包括 n+1 行：

第一行为两个用空格隔开的整数 n 和 m。

第 2 ∽ n+1 行为 n×m 矩阵，其中每行有 m 个用单个空格隔开的非负整数。

## 输出格式

输出文件仅包含1行，为一个整数，即输入矩阵取数后的最大得分。

## 输入输出样例

### 输入 #1
2 3
1 2 3
3 4 2

### 输出 #1
82

## 说明/提示

NOIP 2007 提高第三题。

### 数据范围：

60% 的数据满足：1 ≤ n,m ≤ 30，答案不超过 10的16次方  
100% 的数据满足：1 ≤ n,m ≤ 80，0 <= a{i,j} <= 1000

## 思路

### DP

分别计算出每一行取数m次的最大值，每一行结果之和即最终答案  
对每一行:  
f[i][j][0/1]表示第i次取第j个数，并且是从行首(0)或行尾(1)取的数  
### 转移方程:  
f[i][j][0] = row[j]\*pow(2, i) + max(f[i - 1][j - 1][0], f[i - 1][m - i + j + 1][1])  
f[i][j][1] = row[j]\*pow(2, i) + max(f[i - 1][j + 1][1], f[i - 1][i + j - m - 1][0])  
意思是由上一次取数转移而来，上一次取数可以是行首，也可以是行尾，选择其中大的  
若第i次和第i - 1次在相同方向取数(都从行首，或都从行尾)  
f[i][j][0] 从 f[i - 1][j - 1][0]转移过来  
f[i][j][1] 从 f[i - 1][j + 1][1]转移过来  
若第i次和上一次在不同方向取数:  
由于第i次取数开始之前，已经有 i - 1 个数被取走，  
且第i次要取第j个数，
若第i次取行首(即第j个数是行首)，那么可以计算出当前行尾为 m - i + j, 那么若上一次取行尾，就是取的当前行尾+1，即 m - i + j + 1  
若第i次取行尾(即第j个数是行尾)，那么可以计算出当前行首为 i - m + j, 那么若上一次取行首，就是取的当前行首-1, 即 i - m + j - 1  
需要注意i,j枚举的范围以及特殊情况(见代码注释)  
### 初始化:
f[1][1][0] = row[1]\*2  
f[1][m][1] = row[m]\*2  
另外，对于每次取数，需要单独考虑行首为第1个数，行尾为最后一个数的情况，这两种情况下，上一次取数不能取行首，行尾(见代码注释)


## Code

```c
#include <stdio.h>
#include <stdlib.h>

#define maxn 80
#define maxm 80
int m;

//typedef long long ll;
typedef __uint128_t ll;
ll f[maxm + 1][maxm + 1][2];
int row[maxm + 1];

static ll pow(int i) {
    return (ll)1 << i;
}

static ll max(ll a, ll b) {
    return a > b ? a : b;
}

static ll dp() {
    f[1][1][0] = row[1] * 2;
    f[1][m][1] = row[m] * 2;
    for (int i = 2; i <= m; i++) {
        f[i][1][0] = row[1]*pow(i) + f[i - 1][m - i + 2][1]; //行首为1，上一次取数只能从行尾取
        for (int j = 2; j < i; j++)
            f[i][j][0] = row[j]*pow(i) + max(f[i - 1][j - 1][0], f[i - 1][m - i + j + 1][1]);
        f[i][i][0] = row[i]*pow(i) + f[i - 1][i - 1][0]; //行首为i，说明之前每次取数均从行首取

        f[i][m][1] = row[m]*pow(i) + f[i - 1][i - 1][0]; //行尾为m, 上一次取数只能从行首取
        for (int j = m - 1; j > m - i + 1; j--)
            f[i][j][1] = row[j]*pow(i) + max(f[i - 1][j + 1][1], f[i - 1][i + j - m - 1][0]);
        f[i][m - i + 1][1] = row[m - i + 1]*pow(i) + f[i - 1][m - i + 2][1]; //行尾为m - i + 1，说明之前每次均从行尾取
    }
    ll res = 0;
    for (int i = 1; i <= m; i++) {
        res = max(res, f[m][i][0]);
        res = max(res, f[m][i][1]);
    }
//    printf("%lld\n", res);
    return res;
}

static char *itoa(ll i) {
    size_t len = 0;
    ll j = i;
    while (j) {
        j /= 10;
        len++;
    }
    char *str = malloc((len + 1) * sizeof(char));
    str[len] = '\0';
    while (i) {
        str[--len] = i % 10 + '0';
        i /= 10;
    }
    return str;
}

int main() {
    int n;
    scanf("%d %d", &n, &m);
    ll res = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= m; j++)
            scanf("%d", row + j);
        res += dp();
    }
    if (res) {
        char *strres = itoa(res);
        printf("%s\n", strres);
        free(strres);
    }
    else printf("0");
    exit(0);
}
```

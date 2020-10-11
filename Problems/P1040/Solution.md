# P1040 加分二叉树
[原题地址](https://www.luogu.com.cn/problem/P1040)

## 题目描述

设一个n个节点的二叉树tree的中序遍历为(1,2,3,…,n), 其中数字 1,2,3,…,n为节点编号。每个节点都有一个分数（均为正整数），记第i个节点的分数为di，tree及它的每个子树都有一个加分，任一棵子树subtree（也包含tree本身）的加分计算方法如下：

subtree的左子树的加分 × subtree的右子树的加分 + subtree的根的分数。

若某个子树为空，规定其加分为1，叶子的加分就是叶节点本身的分数。不考虑它的空子树。

试求一棵符合中序遍历为(1,2,3,…,n)且加分最高的二叉树 tree。要求输出

* tree的最高加分。

* tree的前序遍历。

## 输入格式

第1行1个整数n，为节点个数。

第2行n个用空格隔开的整数，为每个节点的分数  
## 输出格式

第1行1个整数，为最高加分（Ans≤4,000,000,000）。

第2行n个用空格隔开的整数，为该树的前序遍历。  
## 输入输出样例
### 输入 #1

> 5  
> 5 7 1 2 10  

### 输出 #1

> 145  
> 3 1 2 4 5  

## 说明/提示

n<30

分数<100

##思路

a[i]表示节点i的分数. 由于中序遍历为1~n, 所以a[i]也就是中序遍历中第i个节点的分数.  
中序遍历已知的情况下，需要确定根节点的位置以分割左右子树.  
很简单的思路就是枚举所有可能的根节点位置，找出最大的加分的二叉树.  
而对于枚举的每一个根节点位置，其前后的区间分别是它的左右子树的中序遍历,  
显然区间DP可以解决这个问题.  
f[i][j]表示以区间[i, j]为中序遍历，可以得到的最大加分  
g[i][j]表示以区间[i, j]为中序遍历，最大的加分对应的二叉树的根节点位置  
f[0][n - 1]就是最终答案(节点编号改为了从0开始)  
g[0][n - 1]就是最大加分二叉树的根节点编号，g[0][g[0][n - 1] - 1]就是左子树的根节点，g[g[0][n - 1] + 1][n - 1]就是右子树的根节点. 递归可输出前序遍历.  

初始化所有叶子节点:  
f[i][i] = a[i], g[i][i] = i

状态转移:  
首先枚举区间长度len，从2到n,  
对每个长度, 枚举start从0到n - len, 表示一个区间开始的位置,  
对每个start, 可以确定区间结束位置end = start + len - 1, 
对区间[start, end], 枚举根节点root从start到end,  
以root为根, 区间[start, end]为中序遍历的二叉树,  
其左子树的中序遍历为区间[start, root - 1], 若start <= root - 1即左子树不空, 则左子树加分为f[start][root - 1], 否则加分为1,  
同理右子树的中序遍历为区间[root + 1, end], 若root + 1 <= end, 则右子树的加分为f[root + 1][end], 否则加分为1,  
那么以root为根, 区间[start, end]为中序遍历的二叉树的加分为 左子树的加分\*右子树的加分+a[root],  
若该二叉树的加分大于f[start][end], 则更新f[start][end], 同时g[start][end]更新为root.  
枚举完root, f[start][end]就计算完毕.  
  
由于len从小到大枚举, 所以计算f[start][end]时, f[start][root - 1]及f[root + 1][end]之前已经计算出了.  

##代码

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxn 30
typedef long long ll;

int a[maxn];
ll f[maxn][maxn];
int g[maxn][maxn];

//递归输出
void print(int start, int end) {
    static bool flag = false;//记录是否输出空格
    if (start > end) return;
    if (flag) putchar(' ');
    else flag = true;
    int root = g[start][end];
    printf("%d", root + 1); //下标从0开始改为从1开始
    print(start, root - 1);
    print(root + 1, end);
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        f[i][i] = a[i];
        g[i][i] = i;
    }
    int end;
    ll left, right;
    //区间DP
    for (int len = 2; len <= n; len++)
        for (int start = 0; start <= n - len; start++) {
            end = start + len - 1;
            for (int root = start; root <= end; root++) {
                left = (start <= root - 1 ? f[start][root - 1] : 1);
                right = (root + 1 <= end ? f[root + 1][end] : 1);
                if (left * right + a[root] > f[start][end]) {
                    f[start][end] = left * right + a[root];
                    g[start][end] = root;
                }
            }
        }
    printf("%lld\n", f[0][n - 1]);
    print(0, n - 1);
    exit(0);
}
```

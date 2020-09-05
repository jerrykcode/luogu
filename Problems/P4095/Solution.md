## 题目背景

“ 寄 没 有 地 址 的 信 ，这 样 的 情 绪 有 种 距 离 ，你 放 着 谁 的 歌 曲 ，是 怎 样 的 心 情 。 能 不 能 说 给 我 听 。”
题目描述

失忆的Eden总想努力地回忆起过去，然而总是只能清晰地记得那种思念的感觉，却不能回忆起她的音容笑貌。

记忆中，她总是喜欢给 Eden 出谜题：在 valentine's day 的夜晚，两人在闹市中闲逛时，望着礼品店里精巧玲珑的各式玩偶，她突发奇想，问了Eden这样的一个问题：有n个玩偶，每个玩偶有对应的价值、价钱，每个玩偶都可以被买有限次，在携带的价钱m固定的情况下，如何选择买哪些玩偶以及每个玩偶买多少个，才能使得选择的玩偶总价钱不超过m，且价值和最大。

众所周知的，这是一个很经典的多重背包问题，Eden很快解决了，不过她似乎因为自己的问题被飞快解决感到了一丝不高兴，于是她希望把问题加难：多次询问，每次询问都将给出新的总价钱，并且会去掉某个玩偶（即这个玩偶不能被选择），再问此时的多重背包的答案（即前一段所叙述的问题）。

这下Eden犯难了，不过Eden不希望自己被难住，你能帮帮他么？

## 输入格式

第一行有一个整数，代表玩偶的个数n，玩偶从0开始编号。

第二行开始后面的n行，每行三个整数，第(i + 2)行的整数 ai,bi,ci，分别表示买一个第i个玩偶需要的价钱，获得的价值以及第i个玩偶的限购次数。

接下来的一行有一个整数q，表示询问次数。

接下来q行，每行两个整数di,ei，表示每个询问去掉的是哪个玩偶（注意玩偶从0开始编号）以及该询问对应的新的总价钱数。（去掉操作不保留，即不同询问互相独立）。


## 输出格式

输出q行，第i行输出对于第i个询问的答案。

## 输入输出样例
### 输入 #1

5  
2 3 4  
1 2 1  
4 1 2  
2 1 1  
3 2 3  
5  
1 10  
2 7  
3 4  
4 8  
0 5  

### 输出 #1

13  
11  
6  
12  
4  



## 样例解释

一共五种玩偶，分别的价钱价值和限购次数为 (2,3,4)，(1,2,1)，(4,1,2)，(2,1,1)，(3,2,3)。

五个询问，以第一个询问为例。

第一个询问表示的是去掉编号为1的玩偶， 且拥有的钱数为10时可以获得的最大价值，则此时剩余玩偶为(2,3,4)，(4,1,2)，(2,1,1)，(3,2,3)，若把编号为0的玩偶买4个（即全买了），然后编号为3的玩偶买一个，则刚好把10元全部花完，且总价值为13。可以证明没有更优的方案了。

注意买某种玩偶不一定要买光。

## 数据规模与约定

* 对于10%的数据，保证 n ≤ 10。
* 另外存在20%的数据，保证n ≤ 100，ci = 1，q ≤ 100。
* 另外存在20%的数据，保证n ≤ 100，q ≤ 100。
* 另外存在30%的数据，保证ci = 1。
* 对于100%的数据，保证 1 ≤ n ≤ 1000，1≤q≤3×10^5，1 ≤ ai,bi,ci ≤ 100，0 ≤ di < n，0≤ ei ≤1000


## 思路

dp[0][i][j]表示从第0个元素到第i个元素，占用空间j，的最大价值  
dp[1][i][j]表示从第 n - 1 个元素到第i个元素，占用空间j，的最大价值  
分别顺序，倒序枚举所有元素，多重背包即可计算出dp值  
对于每个询问，  
max{0 <= j <= e | dp[0][d - 1][j] + dp[1][d + 1][e - j]}即答案  

## Code

```c
#include <stdio.h>
#include <stdlib.h>

#define maxn 1000
#define maxe 1000

int dp[2][maxn][maxe + 1];

typedef struct {
    int index;
    int value;
}Node;
Node queue[maxe];
int head, tail;

void Init() {
    head = 0;
    tail = -1;
}

void Push(int index, int value) {
    while (tail >= head && queue[tail].value < value) {
        tail--;
    }
    tail++;
    queue[tail].index = index;
    queue[tail].value = value;
}

void Check(int index) {
    if (queue[head].index < index) head++;
}

int Top() {
    return queue[head].value;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int val[maxn], cost[maxn], num[maxn];

int main() {
    int n, q, d, e, ans;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &cost[i], &val[i], &num[i]); 
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < cost[i]; j++) {
            Init();
            for (int k = 0; k <= (maxe - j) / cost[i]; k++) {
                Push(k, (i ? dp[0][i - 1][k * cost[i] + j] : 0) - k * val[i]);
                Check(k - num[i]);
                dp[0][i][k * cost[i] + j] = Top() + k * val[i];
            }
        }
    for (int i = n - 1; i >= 0; i--)
        for (int j = 0; j < cost[i]; j++) {
            Init();
            for (int k = 0; k <= (maxe - j) / cost[i]; k++) {
                Push(k, (i < n - 1 ? dp[1][i + 1][k * cost[i] + j] : 0) - k * val[i]);
                Check(k - num[i]);
                dp[1][i][k * cost[i] + j] = Top() + k * val[i];
            }
        }    
    scanf("%d", &q);
    for (int i = 0; i < q; i++) {
        scanf("%d %d", &d, &e);
        ans = 0;
        if (d > 0 && d < n - 1) {
            for (int j = 0; j <= e; j++)
                ans = max(ans, dp[0][d - 1][j] + dp[1][d + 1][e - j]);
        }
        else if (d == 0) ans = dp[1][1][e];
        else ans = dp[0][n - 2][e];
        printf("%d\n", ans);
    }
    exit(0);
}
```

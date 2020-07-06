# P2341 [USACO03FALL][HAOI2006]受欢迎的牛 G

[原题地址](https://www.luogu.com.cn/problem/P2341)


## 题目背景

本题测试数据已修复。

## 题目描述

每头奶牛都梦想成为牛棚里的明星。被所有奶牛喜欢的奶牛就是一头明星奶牛。所有奶牛都是自恋狂，每头奶牛总是喜欢自己的。  
奶牛之间的“喜欢”是可以传递的——如果 AAA 喜欢 BBB，BBB 喜欢 CCC，那么 AAA 也喜欢 CCC。牛栏里共有 NNN 头奶牛，给定一  
些奶牛之间的爱慕关系，请你算出有多少头奶牛可以当明星。  

## 输入格式

第一行：两个用空格分开的整数：N和M。

接下来M行：每行两个用空格分开的整数：A和B，表示A喜欢B。

## 输出格式

一行单独一个整数，表示明星奶牛的数量。  

## 输入输出样例

### 输入 #1

3 3  
1 2  
2 1  
2 3  

## 输出 #1

1  

## 说明/提示

只有3号奶牛可以做明星。

【数据范围】

对于10%的数据，N ≤ 20，M ≤ 50

对于30%的数据，N ≤ 10^3，M ≤ 2×10^4

对于70%的数据，N ≤ 5×10^3，M ≤ 5×10^4

对于100%的数据，1 ≤ N ≤ 10^4，1≤ M≤ 5×10^4


## 思路
若A喜欢B, 则B向A连边，若图中一个顶点可到达其他所有顶点，则该点可以成为明星．  
若图中无环，那么显然，若有多个顶点入度为0，那么不管从谁出发都不能到达所有顶点，　　
而若图中只有1个顶点入度为0，那么从该点出发可以达到其他所有顶点．  
那么对于有环的情况，可以用tarjan缩点，若最后唯一入度为0的点是原来的一个强连通分支，那么该强连通分支中所有顶点皆可  
达到图中其他顶点. 
 
首先使用tarjan缩点，计算出所有强连通分支，用color[v]标记顶点v所属的强连通分支, 并计算出每个强连通分支中的顶点数量，  
这时想象强连通分支作为顶点构成一个新图(代码中不需要真的构建新图)，枚举原图的所有边<v, w>, 若v和w所属的强连  
通分支不同(color[v] != color[w])，那么相当于在新图中，color[v]向color[w]连边，此时`记录color[w]的入度不为0`，  
枚举新图中`入度为0`的点，若有2个或以上，那么输出0，否则输出唯一的入度为0的点所代表的原图中的强连通分量中的顶点数． 

## 代码

```cc
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <vector>
using namespace std;

#define maxn 10000

vector<int> graph[maxn];
int dfn[maxn], low[maxn];
stack<int> s;
bool in_stack[maxn];
int color[maxn];
int num[maxn];
bool has_in[maxn];

int color_cnt;
int dfs_cnt;
void dfs(int v) {
    dfn[v] = low[v] = ++dfs_cnt;
    s.push(v);
    in_stack[v] = true;
    for (int w : graph[v]) {
        if (!dfn[w]) {
            dfs(w);
            if (low[w] < low[v]) low[v] = low[w];
        }
        else if (in_stack[w]) {
            if (dfn[w] < low[v]) low[v] = dfn[w];
        }        
    }
    if (dfn[v] == low[v]) {
        int w;
        while ((w = s.top()) != v) {
            color[w] = color_cnt;
            num[color_cnt]++;
            s.pop();
            in_stack[w] = false;
        }
        color[s.top()] = color_cnt;
        num[color_cnt]++;
        s.pop();
        in_stack[v] = false;
        color_cnt++;
    }
}

int main () {
    int n, m, a, b;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &a, &b);
        a--; b--;
        graph[b].push_back(a);
    }
    //tarjan
    for (int v = 0; v < n; v++)
        if (!dfn[v])
            dfs(v);
    for (int v = 0; v < n; v++)
        for (int w : graph[v]) {
            if (color[v] != color[w]) has_in[color[w]] = true;
        }
    int ans = 0;
    for (int i = 0; i < color_cnt; i++) {
        if (!has_in[i]) {
            if (ans) {
                printf("0\n");
                goto END;
            }
            ans = num[i];
        }
    }
    printf("%d\n", ans);
END:
    return 0;
}
``` 

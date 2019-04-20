# P1550 [USACO08OCT]打井Watering Hole
[原题地址](https://www.luogu.org/problemnew/show/P1550)

## 题目背景

John的农场缺水了！！！

## 题目描述

Farmer John has decided to bring water to his N (1 <= N <= 300) pastures which are conveniently numbered 1..N. He may bring water to a pasture either by building a well in that pasture or connecting the pasture via a pipe to another pasture which already has water.

Digging a well in pasture i costs W_i (1 <= W_i <= 100,000).

Connecting pastures i and j with a pipe costs P_ij (1 <= P_ij <= 100,000; P_ij = P_ji; P_ii=0).

Determine the minimum amount Farmer John will have to pay to water all of his pastures.

POINTS: 400

农民John 决定将水引入到他的n(1<=n<=300)个牧场。他准备通过挖若

干井，并在各块田中修筑水道来连通各块田地以供水。在第i 号田中挖一口井需要花费W_i(1<=W_i<=100,000)元。连接i 号田与j 号田需要P_ij (1 <= P_ij <= 100,000 , P_ji=P_ij)元。

请求出农民John 需要为使所有农场都与有水的农场相连或拥有水井所需要的钱数。

## 输入输出格式

### 输入格式：

* 第1 行为一个整数n。

* 第2 到n+1 行每行一个整数，从上到下分别为W_1 到W_n。

* 第n+2 到2n+1 行为一个矩阵，表示需要的经费（P_ij）。

### 输出格式：

* 只有一行，为一个整数，表示所需要的钱数。

## 输入输出样例

### 输入样例#1： 

> 4  
> 5  
> 4  
> 4  
> 3  
> 0 2 2 2  
> 2 0 3 3  
> 2 3 0 4  
> 2 3 4 0  

### 输出样例#1： 

> 9  
## 说明

John等着用水，你只有1s时间！！！

## 思路

将地面看成一个顶点，它与所有其他顶点有边，边权为打井费用。Prim即可。

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef short Vertex; //顶点
//邻接点结构体
struct AdjNode {
    Vertex v; //邻接点
    int weight; //邻接边权重
    AdjNode(Vertex v, int weight) : v(v), weight(weight) {}
};

//Prim类
class Prim {
public:
    /* 返回最小生成树的权重。*/
    int prim(vector<AdjNode>* graph, int nv);
private:
    //优先队列使用的结构体
    struct PriorityNode {
        Vertex v;
        int dist;
        PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
    };

    struct cmp {
        bool operator() (PriorityNode& a, PriorityNode& b) {
            return a.dist > b.dist;
        }
    };

    //类变量
    int *dist_;
    priority_queue < PriorityNode, vector<PriorityNode>, cmp> q;
};

int Prim::prim(vector<AdjNode>* graph, int nv) {
    //Prim模板
    dist_ = new int[nv];
    fill(dist_, dist_ + nv, NO_VALUE);
    Vertex src = 0;
    dist_[src] = 0;
    q.push(PriorityNode(src, 0));
    int mst_weight = 0; //结果，初始化为0
    Vertex min_v, adj_v;
    int adj_weight;
    while (!q.empty()) {
        min_v = q.top().v;
        q.pop();
        if (min_v != src && dist_[min_v] == 0) continue;
        mst_weight += dist_[min_v];
        dist_[min_v] = 0;
        for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) {
            //遍历邻接点
            adj_v = it->v;
            if (dist_[adj_v]) { //若不在MST内
                adj_weight = it->weight;
                if (adj_weight < dist_[adj_v] || dist_[adj_v] == NO_VALUE) {
                    //更新
                    dist_[adj_v] = adj_weight;
                    q.push(PriorityNode(adj_v, adj_weight));
                }
            }
        }
    }//while
    free(dist_);
    return mst_weight;
}

int main() {
    int n;
    scanf("%d", &n);
    int nv = n + 1; //总顶点数
    vector<AdjNode> *graph = new vector<AdjNode>[nv];
    for (int i = 0, w; i < n; i++) {
        //输入打井费用
        scanf("%d", &w);	
        //与地面(n号顶点)连边
        graph[i].push_back(AdjNode(n, w));
        graph[n].push_back(AdjNode(i, w));		
    }
    for (int i = 0, w; i < n; i++) {
        for (int j = 0; j < n; j++) {
            //输入连通费用
            scanf("%d", &w);
            if (i != j) {
                graph[i].push_back(AdjNode(j, w));
                graph[j].push_back(AdjNode(i, w));
            }
        }
    }
    Prim prim;
    printf("%d", prim.prim(graph, nv));
    for (int i = 0; i < nv; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
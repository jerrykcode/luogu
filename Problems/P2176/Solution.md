# P2176 [USACO14FEB]路障Roadblock
[原题地址](https://www.luogu.org/problemnew/show/P2176)

## 题目描述

每天早晨，FJ从家中穿过农场走到牛棚。农场由 N 块农田组成，农田通过 M 条双向道路连接，每条路有一定长度。FJ 的房子在 1 号田，牛棚在 N 号田。没有两块田被多条道路连接，以适当的路径顺序总是能在农场任意一对田间行走。当FJ从一块田走到另一块时，总是以总路长最短的道路顺序来走。

FJ 的牛呢，总是不安好心，决定干扰他每天早晨的计划。它们在 M 条路的某一条上安放一叠稻草堆，使这条路的长度加倍。牛希望选择一条路干扰使得FJ 从家到牛棚的路长增加最多。它们请你设计并告诉它们最大增量是多少。

## 输入输出格式

### 输入格式：

第 1 行：两个整数 N, M。

第 2 到 M+1 行：第 i+1 行包含三个整数 A_i, B_i, L_i，A_i 和 B_i 表示道路 i 连接的田的编号，L_i 表示路长。

### 输出格式：

第 1 行：一个整数，表示通过使某条路加倍而得到的最大增量。

## 输入输出样例

### 输入样例#1： 

> 5 7  
> 2 1 5  
> 1 3 1  
> 3 2 8  
> 3 5 7  
> 3 4 3  
> 2 4 7  
> 4 5 2  

### 输出样例#1： 

> 2  

## 说明

* 样例说明

若使 3 和 4 之间的道路长加倍，最短路将由 1-3-4-5 变为 1-3-5。

* 数据规模和约定

	* 对于 30%的数据，N <= 70，M <= 1,500。

	* 对于 100%的数据，1 <= N <= 100，1 <= M <= 5,000，1 <= L_i <= 1,000,000。

## 思路

先计算一遍最短路径，设为min_dist，然后遍历路径上所有的边，对每一条边，将其权重增倍，再次计算最短路径，设为tmp_dist，之后将权重还原。tmp_dist - min_dist的最大值即答案。

具体见代码注释

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef unsigned char Vertex; //顶点

//邻接点结构体
struct AdjNode {
    Vertex v; //邻接顶点
    int weight; //邻接边权重
    AdjNode(Vertex v, int weight) : v(v), weight(weight) {}
};

//Dijkstra类
class Dijkstra {
public :
    Dijkstra() : pre_memory_alloc_(false) {}
    ~Dijkstra() { if (pre_memory_alloc_) free(pre_); }

    /* 计算源(0)到终点(nv - 1)的最短路径，用p_min_dist指针传出。nv为顶点数量，record_path表示是否记录路径。*/
    bool dijkstra(vector<AdjNode> *graph, int nv, bool record_path, int *p_min_dist);

    /* 依次得到最后一次调用dijkstra并记录路径时，最短路径上的边，用p_from，p_to指针传出。
       第一次调用本函数得到最短路径上的最后一条边，之后每次调用得到上次调用的前一条边。*/
    bool getMinPath(Vertex *p_from, Vertex *p_to);
private:
    //优先队列使用的结构体
    struct Node {
        Vertex v;
        int dist;
        Node(Vertex v, int dist) : v(v), dist(dist) {}
    };
    struct cmp {
        bool operator() (Node& a, Node& b) {
            return a.dist > b.dist;
        }
    };

    //类变量
    int nv_;
    Vertex src_, des_; //nv_, src_, des_ 在getMinPath中用于记录

    int *dist_;
    bool *collected_;
    Vertex *pre_;
    bool pre_memory_alloc_; //pre_数组分配了内存则为true
};

bool Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, bool record_path, int * p_min_dist) {	
    dist_ = new int[nv];
    fill(dist_, dist_ + nv, NO_VALUE);
    collected_ = new bool[nv];
    fill(collected_, collected_ + nv, false);
    Vertex src = 0, des = nv - 1;
    if (record_path) { //需要记录路径
        //更新nv_，src_，des_
        nv_ = nv;
        src_ = src;
        des_ = des;
        if (pre_memory_alloc_) free(pre_);
        pre_ = new Vertex[nv];
        pre_memory_alloc_ = true;
    }	
    dist_[(size_t)src] = 0;
    priority_queue<Node, vector<Node>, cmp> q;
    q.push(Node(src, dist_[(size_t)src]));
    while (!q.empty()) {
        Vertex min_vertex = q.top().v;
        q.pop();
        if (collected_[(size_t)min_vertex]) continue;
        collected_[(size_t)min_vertex] = true;
        if (min_vertex == des) {
            while (!q.empty()) q.pop();
            break;
        }
        int min_dist = dist_[(size_t)min_vertex];
        for (AdjNode adjNode : graph[min_vertex]) {
            Vertex adj_vertex = adjNode.v;
            int adj_weight = adjNode.weight;
            if (!collected_[(size_t)adj_vertex]) {
                if (min_dist + adj_weight < dist_[(size_t)adj_vertex] || dist_[(size_t)adj_vertex] == NO_VALUE) {
                    dist_[(size_t)adj_vertex] = min_dist + adj_weight;
                    q.push(Node(adj_vertex, dist_[(size_t)adj_vertex]));
                    if (record_path) pre_[(size_t)adj_vertex] = min_vertex; //记录路径
                }
            }
        }
    }//while
    bool result = collected_[(size_t)des];
    *p_min_dist = dist_[(size_t)des];
    free(dist_);
    free(collected_);
    return result;
}

bool Dijkstra::getMinPath(Vertex * p_from, Vertex * p_to) {	
    if (des_ == src_) return false;
    *p_from = pre_[(size_t)des_];
    *p_to = des_;
    des_ = *p_from;
    return true;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    int a_i, b_i, l_i;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a_i, &b_i, &l_i);
        a_i--; b_i--;
        graph[a_i].push_back(AdjNode(b_i, l_i));
        graph[b_i].push_back(AdjNode(a_i, l_i));
    }
    Dijkstra dijkstra;
    int min_dist, tmp_dist, result = 0;
    dijkstra.dijkstra(graph, n, true, &min_dist);
    Vertex from, to;	
    while (dijkstra.getMinPath(&from, &to)) { //遍历最短路径上的所有边
        auto it = graph[(size_t)from].begin();
        for (; it != graph[(size_t)from].end(); it++)
            if (it->v == to) {
                it->weight *= 2; //增倍
                break;
            }
        if (dijkstra.dijkstra(graph, n, false, &tmp_dist))
            if (tmp_dist - min_dist > result) result = tmp_dist - min_dist;
        it->weight /= 2; //还原
    }
    printf("%d", result);
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
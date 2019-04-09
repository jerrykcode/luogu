# P1951 收费站_NOI导刊2009提高（2）
[原题地址](https://www.luogu.org/problemnew/show/P1951)

## 题目描述

在某个遥远的国家里，有n个城市。编号为1，2，3，…,n。

这个国家的政府修建了m条双向的公路。每条公路连接着两个城市。沿着某条公路，开车从一个城市到另一个城市，需要花费一定的汽油。

开车每经过一个城市，都会被收取一定的费用（包括起点和终点城市）。所有的收费站都在城市中，在城市间的公路上没有任何的收费站。

小红现在要开车从城市u到城市v（1<=u,v<=n）。她的车最多可以装下s升的汽油。在出发的时候，车的油箱是满的，并且她在路上不想加油。

在路上，每经过一个城市，她都要交一定的费用。如果某次交的费用比较多，她的心情就会变得很糟。所以她想知道，在她能到达目的地的前提下，她交的费用中最多的一次最少是多少。这个问题对于她来说太难了，于是她找到了聪明的你，你能帮帮她吗？

## 输入输出格式

### 输入格式：

第一行5个正整数，n，m，u，v，s，分别表示有n个城市，m条公路，从城市u到城市v，车的油箱的容量为s升。

接下来的有n行，每行1个整数，fi表示经过城市i，需要交费fi元。

再接下来有m行，每行3个正整数，ai，bi，ci（1<=ai,bi<=n）,表示城市ai和城市bi之间有一条公路，如果从城市ai到城市bi，或者从城市bi到城市ai，需要ci升的汽油。

### 输出格式：

仅一个整数，表示小红交费最多的一次的最小值。

如果她无法到达城市v，输出-1.

## 输入输出样例

### 输入样例#1： 

> 4 4 2 3 8  
> 8  
> 5  
> 6  
> 10  
> 2 1 2  
> 2 4 1  
> 1 3 4  
> 3 4 3  

### 输出样例#1： 

> 8  

## 说明

【数据规模】

* 对于60%的数据，满足n<=200,m<=10000,s<=200

* 对于100%的数据，满足n<=10000,m<=50000,s<=1000000000

* 对于100%的数据，满足ci<=1000000000,fi<=1000000000,可能有两条边连接着相同的城市。

## 思路

见[P1462 通往奥格瑞玛的道路-Solution](https://github.com/jerrykcode/luogu/blob/master/Problems/P1462/Solution.md)

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define NO_VALUE -1
typedef long long LL;

//邻接点结构体
struct AdjNode {
    int v; //邻接点顶点
    int weight; //邻接边权重
    AdjNode(int v, int weight) : v(v), weight(weight) {}
};

//Dijkstra类
class Dijkstra {
public:
    Dijkstra() {}
    ~Dijkstra() {}

    /*
        计算src到des的最短路径。
        @param graph 图
        @param nv 顶点数
        @param src 起点(源)
        @param des 终点
        @param f 点权数组，f[i]为i的点权
        @param max_f 最短路径上的顶点不能超过的最大点权
        @param P_min_dist 指针，用于传出最短距离
        @return 若在点权不超过max_f的情况下，src可到达des则返回true，否则返回false
    */
    bool dijkstra(vector<AdjNode> *graph, int nv, int src, int des, int *f, int max_f, int *p_min_dist);
private:
    //优先队列使用的结构体
    struct Node {
        int v;
        LL dist;
        Node(int v, LL dist) : v(v), dist(dist) {}
    };
    struct cmp {
        bool operator() (Node& a, Node& b) {
            return a.dist > b.dist;
        }
    };

    //类变量
    LL *dist_;
    bool *collected_;
    priority_queue<Node, vector<Node>, cmp> queue_;
};

bool Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int *f, int max_f, int * p_min_dist) {
    if (f[src] > max_f || f[des] > max_f) return false;
    dist_ = new LL[nv];
    fill(dist_, dist_ + nv, NO_VALUE);
    collected_ = new bool[nv];
    fill(collected_, collected_ + nv, false);
    dist_[src] = 0;
    queue_.push(Node(src, dist_[src]));
    int min_vertex, adj_vertex, adj_weight;
    LL tmp_dist;
    while (!queue_.empty()) {
        min_vertex = queue_.top().v;
        queue_.pop();
        if (collected_[min_vertex]) continue;
        collected_[min_vertex] = true;
        if (min_vertex == des) {
            while (!queue_.empty()) queue_.pop();
            break;
        }
        for (AdjNode adj_node : graph[min_vertex]) { //遍历邻接点
            adj_vertex = adj_node.v;
            adj_weight = adj_node.weight;
            if (!collected_[adj_vertex] && f[adj_vertex] <= max_f) {
                tmp_dist = dist_[min_vertex] + adj_weight;
                if (tmp_dist < dist_[adj_vertex] || dist_[adj_vertex] == NO_VALUE) {
                    dist_[adj_vertex] = tmp_dist;
                    queue_.push(Node(adj_vertex, dist_[adj_vertex]));
                }
            }
        }
    } //while
    *p_min_dist = dist_[des];
    free(dist_);
    bool result = collected_[des];
    free(collected_);
    return result;
}

/* 二分查找。*/
int binarySearch(int *arr, int n, int val) {
    int left = 0, right = n - 1, mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (arr[mid] == val) return mid;
        else if (arr[mid] < val) left = mid + 1;
        else right = mid - 1;
    }
    return NO_VALUE;
}

int main() {
    int n, m, u, v, s;
    scanf("%d %d %d %d %d", &n, &m, &u, &v, &s);	
    u--; v--; //改为从0开始编号
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    int *f = new int[n];
    for (int i = 0; i < n; i++)
        scanf("%d", f + i); //输入点权
    int *f_sorted = new int[n];
    memcpy(f_sorted, f, n * sizeof(int));
    sort(f_sorted, f_sorted + n); //排序点权
    int a, b, c;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a, &b, &c);
        a--; b--;
        graph[a].push_back(AdjNode(b, c));
        graph[b].push_back(AdjNode(a, c));
    }
    Dijkstra dijkstra;
    int min_dist;
    int left = max(binarySearch(f_sorted, n, f[u]), binarySearch(f_sorted, n, f[v])); //点权一定大于等于src和des的点权
    int right = n - 1, mid, last = NO_VALUE;
    //二分
    while (left <= right) {
        mid = (left + right) / 2;
        if (dijkstra.dijkstra(graph, n, u, v, f, f_sorted[mid], &min_dist) && min_dist < s) {
            last = mid;
            right = mid - 1;
        }
        else left = mid + 1;
    }
    last == NO_VALUE ? printf("-1") : printf("%d", f_sorted[last]);
    free(f);
    free(f_sorted);
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
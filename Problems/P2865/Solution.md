# P2865 [USACO06NOV]路障Roadblocks
[原题地址](https://www.luogu.org/problemnew/show/P2865)


题目描述
----

Bessie has moved to a small farm and sometimes enjoys returning to visit one of her best friends. She does not want to get to her old home too quickly, because she likes the scenery along the way. She has decided to take the second-shortest rather than the shortest path. She knows there must be some second-shortest path.

The countryside consists of R (1 ≤ R ≤ 100,000) bidirectional roads, each linking two of the N (1 ≤ N ≤ 5000) intersections, conveniently numbered 1..N. Bessie starts at intersection 1, and her friend (the destination) is at intersection N.

The second-shortest path may share roads with any of the shortest paths, and it may backtrack i.e., use the same road or intersection more than once. The second-shortest path is the shortest path whose length is longer than the shortest path(s) (i.e., if two or more shortest paths exist, the second-shortest path is the one whose length is longer than those but no longer than any other path).

贝茜把家搬到了一个小农场，但她常常回到FJ的农场去拜访她的朋友。贝茜很喜欢路边的风景，不想那么快地结束她的旅途，于是她每次回农场，都会选择第二短的路径，而不象我们所习惯的那样，选择最短路。 贝茜所在的乡村有R(1<=R<=100,000)条双向道路，每条路都联结了所有的N(1<=N<=5000)个农场中的某两个。贝茜居住在农场1，她的朋友们居住在农场N（即贝茜每次旅行的目的地）。 贝茜选择的第二短的路径中，可以包含任何一条在最短路中出现的道路，并且，一条路可以重复走多次。当然咯，第二短路的长度必须严格大于最短路（可能有多条）的长度，但它的长度必须不大于所有除最短路外的路径的长度。

输入输出格式
------

输入格式：

Line 1: Two space-separated integers: N and R

Lines 2..R+1: Each line contains three space-separated integers: A, B, and D that describe a road that connects intersections A and B and has length D (1 ≤ D ≤ 5000)

输出格式：

Line 1: The length of the second shortest path between node 1 and node N

输入输出样例
------

输入样例#1：?复制

> 4 4  
> 1 2 100  
> 2 4 200  
> 2 3 250  
> 3 4 100  

输出样例#1：?复制

> 450  

说明
--

Two routes: 1 -> 2 -> 4 (length 100+200=300) and 1 -> 2 -> 3 -> 4 (length 100+250+100=450)

## 思路

A*求次短路

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef unsigned short Vertex; //顶点

//邻接点结构体
struct AdjNode {
    Vertex adj_v; //邻接点
    int adj_weight; //邻接边权重
    AdjNode(Vertex adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//计算次短路的类
class SecondShortestPath {
public:
    /* 计算次短路。
        @param graph 图
        @param nv 顶点数
        @param src 源
        @param des 终点
        @return int 次短路长度
    */
    int getSecondShortestPath(vector<AdjNode> * graph, int nv, int src, int des);
private:
    /* 计算源到每个顶点的最短距离。
        @param graph 图
        @param nv 顶点数
        @param src 源
        @param dist 存储最短距离的数组(传出)
        @return void
    */
    void dijkstra(vector<AdjNode> * graph, int nv, int src, int *dist);

    /* A*计算次短路。
        @param graph 图
        @param nv 顶点数
        @param src 源
        @param des 终点
        @param h 估值数组
         @return int 次短路长度
    */
    int astar(vector<AdjNode>* graph, int nv, int src, int des, int *h);

    //优先队列使用的结构体
    //dijkstra优先队列使用
    struct PriorityNode {
        Vertex v;
        int dist;
        PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
    };

    //A*优先队列使用
    struct AStarNode : PriorityNode {
        //PriorityNode::dist 为f
        int g;
        AStarNode(Vertex v, int g, int f) : PriorityNode(v, f), g(g) {}
    };

    struct cmp {
        bool operator () (PriorityNode& a, PriorityNode& b) {
            return a.dist > b.dist;
        }
    };	
};

int SecondShortestPath::getSecondShortestPath(vector<AdjNode>* graph, int nv, int src, int des) {
    int *h = new int[nv];
    //dijkstra计算des到每个顶点的最短距离，作为h
    dijkstra(graph, nv, des, h);
    //计算次短路
    int result = astar(graph, nv, src, des, h);
    free(h);
    return result;
}

void SecondShortestPath::dijkstra(vector<AdjNode>* graph, int nv, int src, int *dist) {
    fill(dist, dist + nv, NO_VALUE);
    bool *collected = new bool[nv];
    fill(collected, collected + nv, false);
    dist[src] = 0;
    priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
    q.push(PriorityNode(src, 0));
    Vertex min_v, adj_v;
    int adj_weight, tmp_dist;
    while (!q.empty()) {
        min_v = q.top().v;
        q.pop();
        if (collected[min_v]) continue;
        collected[min_v] = true;
        for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) { 
            //遍历邻接点
            adj_v = it->adj_v;
            if (!collected[adj_v]) {
                adj_weight = it->adj_weight;
                tmp_dist = dist[min_v] + adj_weight;
                if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
                    dist[adj_v] = tmp_dist;
                    q.push(PriorityNode(adj_v, tmp_dist));
                }
            }
        } //for
    } //while
    free(collected);
}

int SecondShortestPath::astar(vector<AdjNode>* graph, int nv, int src, int des, int *h) {
    priority_queue<AStarNode, vector<AStarNode>, cmp> q;
    q.push(AStarNode(src, 0, h[src]));
    Vertex min_v, adj_v;
    int min_g, tmp_g, count = 0;
    while (!q.empty()) {
        min_v = q.top().v;
        min_g = q.top().g;
        q.pop();
        if (min_v == des) {
            if (++count == 2) {
                //第二次弹出des
                while (!q.empty()) q.pop();
                return min_g;
            }
        }
        for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) {
            //遍历邻接点
            adj_v = it->adj_v;
            tmp_g = min_g + it->adj_weight;
            q.push(AStarNode(adj_v, tmp_g, tmp_g + h[adj_v]));
        }
    } //while
    return NO_VALUE;
}

int main() {
    int n, r;
    scanf("%d %d", &n, &r);
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    for (int i = 0, a, b, d; i < r; i++) {
        scanf("%d %d %d", &a, &b, &d);
        a--; b--;
        graph[a].push_back(AdjNode(b, d));
        graph[b].push_back(AdjNode(a, d));
    }
    SecondShortestPath ssp;
    printf("%d", ssp.getSecondShortestPath(graph, n, 0, n - 1));
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
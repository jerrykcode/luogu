# P2829 大逃离
[原题地址](https://www.luogu.org/problemnew/show/P2829)

题目背景
----

zrz走进了一个奇葩的迷宫，他发现自己迷路了，想逃出来，他好不容易数完了所有的路，累的快晕了，只好叫你帮忙咯。

题目描述
----

这是一棵有n个节点的图，有m条双向边，每一条路有w个单位距离，zrz在1的位置，出口在n的位置，不过zrz脑子出了点bug，于是不想走最短的路，想走第2短的路，第2短路径允许与最短路径有重边，然后也可以重复通过一些节点和路，注意如果有多条路径都是最短路径，那么他们都不能叫第2短路径。但是zrz觉得如果接下来进入的一个节点所直接连接的地方小于k个（起点和终点除外），那么他就不敢进去。

输入输出格式
------

输入格式：

第一行3个数：n,m,k

接下来m行：每行3个数，u，v，w。表示从u到v有一条权值为w的边。（u,v<=n，w<=10000）

输出格式：

一个数：表示从s走到t的第2短路的值，如果不存在，输出-1

输入输出样例
------

### 输入样例#1：

> 4 4 1  
> 1 2 100  
> 2 4 200  
> 2 3 250  
> 3 4 100  

### 输出样例#1：

> 450  

### 输入样例#2：

> 4 4 3  
> 1 2 100  
> 2 4 200  
> 2 3 250  
> 3 4 100  

### 输出样例#2：

500

说明
--

对于50%的数据：n<=10,m<=10

对于90%的数据：n<=1000,m<=20000

对于100%的数据：n<=5000，m<=100000

另外，k比较小

样例2最短路径是300（1-2-4）。因为从2无法走到3（3连接到的节点只有2个），所以可以1-2-1-2-4，第二短路为500。

## 思路

A*求次短路，反向Dijkstra计算出的每个顶点到终点的最短距离为估价函数。

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short Vertex; //顶点
#define NO_VALUE -1

//邻接点结构体
struct AdjNode {
    Vertex adj_v; //邻接点
    short adj_weight; //邻接边权重
    AdjNode(Vertex adj_v, short adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//计算无向图次短路的类
class SecondShortestPath {
public:
    /* 计算无向图次短路。
        @param graph 图
        @param nv 顶点数
        @param src 源
        @param des 终点
        @param num 数组，存储每个顶点的邻接点数（graph可能有重边）
        @param k 扩展邻接点时，若邻接点不是src和des，且其邻接点数小于k，则不扩展该邻接点
        @param p_second_shortest_dist 指针，传出次短路长度
        @return bool 若次短路存在则返回true
    */
    bool getSecondShortestPath(vector<AdjNode> *graph, int nv, int src, int des, int *num, int k, int *p_second_shortest_dist);
private:
    /* Dijkstra计算最短路。
        @param graph 图
        @param nv 顶点数
        @param src 源
        @param des 终点
        @param num 数组，存储每个顶点的邻接点数（graph可能有重边）
        @param k 扩展邻接点时，若邻接点不是src和des，且其邻接点数小于k，则不扩展该邻接点
        @param dist 数组，传出src到每一个顶点的最短距离
        @return bool 若src可达des则返回true
    */
    bool dijkstra(vector<AdjNode> *graph, int nv, int src, int des, int *num, int k, int *dist);

    /* A*计算次短路。
        @param graph 图
        @param h 数组 A*算法的估值函数
        @param nv 顶点数
        @param src 源
        @param des 终点
        @param num 数组，存储每个顶点的邻接点数（graph可能有重边）
        @param k 扩展邻接点时，若邻接点不是src和des，且其邻接点数小于k，则不扩展该邻接点
        @param p_second_shortest_dist 指针，传出次短路长度
        @return bool 若次短路存在则返回true
    */
    bool astar(vector<AdjNode> *graph, int *h, int nv, int src, int des, int *num, int k, int *p_second_shortest_dist);

    //Dijkstra优先队列使用的结构体
    struct PriorityNode {
        Vertex v;
        int dist;
        PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
    };

    //A*优先队列使用的结构体，继承了PriorityNode
    struct AstarNode : public PriorityNode {
        //f = dist
        int g;
        AstarNode(Vertex v, int g, int f) : PriorityNode(v, f), g(g) {}
    };

    //优先队列用于比较的结构体
    struct cmp {
        bool operator () (PriorityNode& a, PriorityNode& b) {
            return a.dist > b.dist;
        }
    };
};

bool SecondShortestPath::getSecondShortestPath(vector<AdjNode>* graph, int nv, int src, int des, int *num, int k, int *p_second_shortest_dist) {
    int *h = new int[nv];
    bool result;
    if (dijkstra(graph, nv, des, src, num, k, h)) { //反向计算最短路，作为A*的估值函数
        result = astar(graph, h, nv, src, des, num, k, p_second_shortest_dist); //A*
    }
    else result = false;
    free(h);
    return result;
}

bool SecondShortestPath::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int *num, int k, int * dist) {
    fill(dist, dist + nv, NO_VALUE);
    bool *collected = new bool[nv];
    fill(collected, collected + nv, false);
    dist[src] = 0;
    priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
    q.push(PriorityNode(src, 0));
    Vertex top_v, adj_v;
    int tmp_dist;
    while (!q.empty()) {
        top_v = q.top().v;
        q.pop();
        if (collected[top_v]) continue;
        collected[top_v] = true;
        for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
            adj_v = it->adj_v;
            if ((num[adj_v] >= k || adj_v == des) && !collected[adj_v]) { //邻接点的邻接点数目需要>=k，或者邻接点就是des
                tmp_dist = dist[top_v] + (int)it->adj_weight;
                if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
                    dist[adj_v] = tmp_dist;
                    q.push(PriorityNode(adj_v, tmp_dist));
                }
            }
        } //for
    } //while
    free(collected);
    return dist[des] != NO_VALUE;
}

bool SecondShortestPath::astar(vector<AdjNode>* graph, int *h, int nv, int src, int des, int *num, int k, int * p_second_shortest_dist) {
    priority_queue<AstarNode, vector<AstarNode>, cmp> q;
    q.push(AstarNode(src, 0, h[src]));
    Vertex top_v, adj_v;
    int top_g, last_g = NO_VALUE, tmp_g, count = 0;
    while (!q.empty()) {
        top_v = q.top().v;
        top_g = q.top().g;
        q.pop();
        if (top_v == des && top_g != last_g) { //弹出des，且des的g值和上次弹出des的g值不同
            if (++count == 2) { //计数
                while (!q.empty()) q.pop();
                *p_second_shortest_dist = top_g;
                return true;
            }
            last_g = top_g; //更新last_g为这次弹出des的g值
        }
        for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
            adj_v = it->adj_v;
            if (num[adj_v] >= k || adj_v == src || adj_v == des) { //邻接点需要>=k或者邻接点就是src或者des
                tmp_g = top_g + (int)it->adj_weight;
                q.push(AstarNode(adj_v, tmp_g, tmp_g + h[adj_v]));
            }
        }
    }
    return false;
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    int *num = new int[n];
    fill(num, num + n, 0);
    for (int i = 0, u, v, w; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        u--; v--;
        graph[u].push_back(AdjNode(v, w));
        graph[v].push_back(AdjNode(u, w));
    }
    bool *appeared = new bool[n];	
    for (int i = 0; i < n; i++) {
        fill(appeared, appeared + n, false);
        for (auto it = graph[i].begin(); it != graph[i].end(); it++) {
            if (appeared[it->adj_v]) continue;
            else {
                num[i]++;
                appeared[it->adj_v] = true;
            }
        }
    }
    SecondShortestPath ssp;
    int second_shortest_dist;
    if (ssp.getSecondShortestPath(graph, n, 0, n - 1, num, k, &second_shortest_dist)) {
        printf("%d", second_shortest_dist);
    }
    else printf("-1");
    free(num);
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);	
    return 0;
}


```
# P4568 [JLOI2011]飞行路线
[原题地址](https://www.luogu.org/problemnew/show/P4568)

题目描述
----

Alice和Bob现在要乘飞机旅行，他们选择了一家相对便宜的航空公司。该航空公司一共在nn个城市设有业务，设这些城市分别标记为00到n-1n-1，一共有mm种航线，每种航线连接两个城市，并且航线有一定的价格。

Alice和Bob现在要从一个城市沿着航线到达另一个城市，途中可以进行转机。航空公司对他们这次旅行也推出优惠，他们可以免费在最多kk种航线上搭乘飞机。那么Alice和Bob这次出行最少花费多少？

输入输出格式
------

输入格式：

数据的第一行有三个整数，n,m,kn,m,k，分别表示城市数，航线数和免费乘坐次数。\
第二行有两个整数，s,ts,t，分别表示他们出行的起点城市编号和终点城市编号。\
接下来有m行，每行三个整数，a,b,ca,b,c，表示存在一种航线，能从城市aa到达城市bb，或从城市bb到达城市aa，价格为cc。

输出格式：

只有一行，包含一个整数，为最少花费。

输入输出样例
------

输入样例#1：?复制

5 6 1
0 4
0 1 5
1 2 5
2 3 5
3 4 5
2 3 3
0 2 100

输出样例#1：?复制

8

说明
--

对于30%的数据,2 \le n \le 50,1 \le m \le 300,k=02≤n≤50,1≤m≤300,k=0;\
对于50%的数据,2 \le n \le 600,1 \le m \le 6000,0 \le k \le 12≤n≤600,1≤m≤6000,0≤k≤1;\
对于100%的数据,2 \le n \le 10000,1 \le m \le 50000,0 \le k \le 102≤n≤10000,1≤m≤50000,0≤k≤10,0 \le s,t<n,0 \le a,b<n,a\neq b,0 \le c \le 10000≤s,t<n,0≤a,b<n,a≠b,0≤c≤1000

2018.12.10 增加一组 hack 数据


## 思路

分层图最短路模板

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

//邻接点结构体
struct AdjNode {
    int v; //邻接点
    int weight; //邻接边权重
    AdjNode(int v, int weight) : v(v), weight(weight) {}
};

//Dijkstra类
class Dijkstra {
public:
    /* 计算最短路径
     @param graph 邻接表表示的图
     @param nv 顶点数
     @param src 源
     @param des 终点
     @param max_free_time 最大免费次数，需要小于图的边数
     @return 源到终点不超过max_free_time次免费的最短距离
     */
    int dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int max_free_time);
private:
    //优先队列使用的结构体
    struct PriorityNode {
        int v;
        int dist;
        int free_time; //到达v时使用免费的次数，即分层图中的层
        PriorityNode(int v, int dist, int free_time) : v(v), dist(dist), free_time(free_time) {}
    };
    struct cmp {
        bool operator () (PriorityNode& a, PriorityNode& b) {
            return a.dist > b.dist;
        }
    };

    //类变量
    int **dist_; //dist_[i][j]为从src到i不超过j次免费的最短距离
    bool **collected_; // collected_[i][j]为true则src到i不超过j次免费的最低距离已确定
    priority_queue<PriorityNode, vector<PriorityNode>, cmp> queue_;
};

int Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int max_free_time) {
    dist_ = new int*[nv];
    collected_ = new bool*[nv];
    for (int i = 0; i < nv; i++) {
        //dist_[i][j]为从src到i不超过j次免费的最短距离
        dist_[i] = new int[max_free_time + 1];
        fill(dist_[i], dist_[i] + max_free_time + 1, NO_VALUE);
        //collected_[i][j]为true则src到i不超过j次免费的最低距离已确定
        collected_[i] = new bool[max_free_time + 1];
        fill(collected_[i], collected_[i] + max_free_time + 1, false);
    }	
    dist_[src][0] = 0; //src到src，0次免费，距离为0
    queue_.push(PriorityNode(src, 0, 0));
    int min_v, min_free_time, min_dist, adj_v, adj_weight, tmp_dist;
    while (!queue_.empty()) {
        //队列中dist最小的出队列
        min_v = queue_.top().v; 
        min_dist = queue_.top().dist;
        min_free_time = queue_.top().free_time;
        queue_.pop();
        if (collected_[min_v][min_free_time]) continue;
        collected_[min_v][min_free_time] = true;
        if (min_v == des && min_free_time == max_free_time) {
            while (!queue_.empty()) queue_.pop();
            break;
        }
        for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) { 
            //遍历邻接点
            adj_v = it->v;
            adj_weight = it->weight;			
            if (!collected_[adj_v][min_free_time]) {
                tmp_dist = min_dist + adj_weight; //若min_v到adj_v不免费
                if (tmp_dist < dist_[adj_v][min_free_time] || dist_[adj_v][min_free_time] == NO_VALUE) {
                    dist_[adj_v][min_free_time] = tmp_dist;
                    queue_.push(PriorityNode(adj_v, tmp_dist, min_free_time));
                }
            }
            if (min_free_time < max_free_time && !collected_[adj_v][min_free_time + 1]) {
                tmp_dist = min_dist; //若min_v到adj_v免费
                if (tmp_dist < dist_[adj_v][min_free_time + 1] || dist_[adj_v][min_free_time + 1] == NO_VALUE) { //免费次数增1
                    dist_[adj_v][min_free_time + 1] = tmp_dist;
                    queue_.push(PriorityNode(adj_v, tmp_dist, min_free_time + 1));
                }
            }
        } //for
    } //while
    int result = dist_[des][max_free_time];
    for (int i = 0; i < nv; i++) {
        free(dist_[i]);
        free(collected_[i]);
    }
    free(dist_);
    free(collected_);
    return result;
}

int main() {
    int n, m, k, s, t;
    scanf("%d %d %d", &n, &m, &k);
    scanf("%d %d", &s, &t);
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    int a, b, c;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a, &b, &c);
        graph[a].push_back(AdjNode(b, c));
        graph[b].push_back(AdjNode(a, c));
    }
    Dijkstra dijkstra;
    printf("%d", dijkstra.dijkstra(graph, n, s, t, min(m, k))); //注意m < k的情况
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
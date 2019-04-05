# P1462 通往奥格瑞玛的道路
[原题地址](https://www.luogu.org/problemnew/show/P1462)

## 题目背景

在艾泽拉斯大陆上有一位名叫歪嘴哦的神奇术士，他是部落的中坚力量

有一天他醒来后发现自己居然到了联盟的主城暴风城

在被众多联盟的士兵攻击后，他决定逃回自己的家乡奥格瑞玛

## 题目描述

在艾泽拉斯，有n个城市。编号为1,2,3,...,n。

城市之间有m条双向的公路，连接着两个城市，从某个城市到另一个城市，会遭到联盟的攻击，进而损失一定的血量。

每次经过一个城市，都会被收取一定的过路费（包括起点和终点）。路上并没有收费站。

假设1为暴风城，n为奥格瑞玛，而他的血量最多为b，出发时他的血量是满的。

歪嘴哦不希望花很多钱，他想知道，在可以到达奥格瑞玛的情况下，他所经过的所有城市中最多的一次收取的费用的最小值是多少。

## 输入输出格式

### 输入格式：

第一行3个正整数，n，m，b。分别表示有n个城市，m条公路，歪嘴哦的血量为b。

接下来有n行，每行1个正整数，fi。表示经过城市i，需要交费fi元。

再接下来有m行，每行3个正整数，ai，bi，ci(1<=ai，bi<=n)。表示城市ai和城市bi之间有一条公路，如果从城市ai到城市bi，或者从城市bi到城市ai，会损失ci的血量。

### 输出格式：

仅一个整数，表示歪嘴哦交费最多的一次的最小值。

如果他无法到达奥格瑞玛，输出AFK。

## 输入输出样例

### 输入样例#1： 

> 4 4 8  
> 8  
> 5  
> 6  
> 10  
> 2 1 2  
> 2 4 1  
> 1 3 4  
> 3 4 3  

### 输出样例#1： 

> 10  


## 说明

* 对于60%的数据，满足n <= 200，m <= 10000，b <= 200

* 对于100%的数据，满足n <= 10000，m <= 50000，b <= 1000000000

* 对于100%的数据，满足ci <= 1000000000，fi <= 1000000000，可能有两条边连接着相同的城市。

## 思路

[参考](https://www.luogu.org/problemnew/solution/P1462)

即找一条源到终点的路径，路径长度小于b，同时要求路径上的顶点的最大权重越小越好。求这个尽可能小的点权。对于任意一个权重f，可以计算源到终点的最短路径(路径上的顶点最大权重不超过f)，若源可以到达终点，且这个最短路径小于b，那么f可以继续减小，否则需要增加f。可以二分确定f的值。

由于f必然大于等于起点和终点的点权，所以二分的左端left为源和终点点权重的较大值，而右端right为所有顶点的最大点权。若原点权数组为f，排序后得到f_sorted数组，则max(f[src], f[des])在f_sorted中的位置即为left，n - 1为right，对f_sorted二分即可。

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
    int v; //邻接点 
    int weight; //邻接边权重
    AdjNode(int v, int weight) : v(v), weight(weight) {}
};

//Dijkstra类
class Dijkstra {
public:
    Dijkstra() {}
    ~Dijkstra() {}

    /* 计算从源(0)到终点(nv - 1)的最短路径，路径上的顶点权重不超过max_f。*/
    LL dijkstra(vector<AdjNode> *graph, int nv, int *f, int max_f);
private:
    //优先队列使用的结构体
    struct Node {
        int v;
        LL dist;
        Node(int v, LL dist) : v(v), dist(dist) {}
    };
    struct cmp {
        bool operator () (Node& a, Node& b) {
            return a.dist > b.dist;
        }
    };

    //类变量
    LL *dist_;
    bool *collected_;
};

LL Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int * f, int max_f) {
    int src = 0, des = nv - 1;
    if (f[src] > max_f || f[des] > max_f) return NO_VALUE;
    dist_ = new LL[nv];
    fill(dist_, dist_ + nv, NO_VALUE);
    collected_ = new bool[nv];
    fill(collected_, collected_ + nv, false);	
    dist_[src] = 0;
    priority_queue<Node, vector<Node>, cmp> q;
    q.push(Node(src, dist_[src]));
    while (!q.empty()) {
        int minV = q.top().v;
        q.pop();
        if (collected_[minV]) continue;
        collected_[minV] = true;
        if (minV == des) {
            break;
        }
        for (AdjNode adjNode : graph[minV]) { //遍历minV的邻接点
            int adjVertex = adjNode.v;
            int adjWeight = adjNode.weight;
            if (!collected_[adjVertex] && f[adjVertex] <= max_f) {
                if (dist_[minV] + adjWeight < dist_[adjVertex] || dist_[adjVertex] == NO_VALUE) {
                    dist_[adjVertex] = dist_[minV] + adjWeight;
                    q.push(Node(adjVertex, dist_[adjVertex]));
                }
            }
        }
    } //while
    //若无法到达des，则dist_[des]的值为NO_VALUE(-1)
    LL result = dist_[des];
    free(dist_);
    free(collected_);
    return result;
}

/* 二分查找。*/
int binary_search(int *arr, int n, int val) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid] == val) return mid;
        else if (arr[mid] < val) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    int n, m, b;
    scanf("%d %d %d", &n, &m, &b);
    if (n == 0) {
        printf("0");
        return 0;
    }
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    int *f = new int[n];
    for (int i = 0; i < n; i++)
        scanf("%d", f + i);
    int v, w, c;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &v, &w, &c);
        v--; w--;
        graph[v].push_back(AdjNode(w, c));
        graph[w].push_back(AdjNode(v, c));
    }
    int *f_sorted = new int[n];
    memcpy(f_sorted, f, n * sizeof(int));
    sort(f_sorted, f_sorted + n);
    int left = binary_search(f_sorted, n, max(f[0], f[n - 1])), right = n - 1; //至少需要src和des的权重
    Dijkstra dijkstra;
    LL min_dist;
    int mid;
    //二分
    int last = NO_VALUE;
    while (left <= right) {
        mid = (left + right) / 2;
        min_dist = dijkstra.dijkstra(graph, n, f, f_sorted[mid]);
        if (min_dist == NO_VALUE || min_dist >= b) left = mid + 1; //扩大权重范围
        else {
            last = mid;
            right = mid - 1; //缩小权重范围
        }
    }
    last == NO_VALUE ? printf("AFK") : printf("%d", f_sorted[last]);
    free(f);
    free(f_sorted);
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
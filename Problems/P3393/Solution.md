# P3393 逃离僵尸岛

[原题地址](https://www.luogu.org/problemnew/show/P3393)


## 题目描述

小a住的国家被僵尸侵略了！小a打算逃离到该国唯一的国际空港逃出这个国家。

该国有N个城市，城市之间有道路相连。一共有M条双向道路。保证没有自环和重边。

K个城市已经被僵尸控制了，如果贸然闯入就会被感染TAT...所以不能进入。由其中任意城市经过不超过S条道路就可以到达的别的城市，就是危险城市。换句话说只要某个没有被占城市到某个被占城市不超过s距离，就是危险。

小a住在1号城市，国际空港在N号城市，这两座城市没有被侵略。小a走每一段道路（从一个城市直接到达另外一个城市）得花一整个白天，所以晚上要住旅店。安全的的城市旅馆比较便宜要P元，而被危险的城市，旅馆要进行安保措施，所以会变贵，为Q元。所有危险的城市的住宿价格一样，安全的城市也是。在1号城市和N城市，不需要住店。

小a比较抠门，所以他希望知道从1号城市到N号城市所需要的最小花费。

输入数据保证存在路径，可以成功逃离。输入数据保证他可以逃离成功。

输入输出格式
------

**输入格式：**

第一行4个整数(N,M,K,S)

第二行2个整数(P,Q)

接下来K行，ci，表示僵尸侵占的城市

接下来M行，ai,bi，表示一条无向边

**输出格式：**

一个整数表示最低花费

输入输出样例
------

**输入样例#1：** 

13 21 1 1  
1000 6000  
7  
1 2  
3 7  
2 4  
5 8  
8 9  
2 5  
3 4  
4 7  
9 10  
10 11  
5 9  
7 12  
3 6  
4 5  
1 3  
11 12  
6 7  
8 11  
6 13  
7 8  
12 13  

**输出样例#1：** 

11000  

说明
--

![](https://cdn.luogu.org/upload/pic/2681.png)

对于20%数据，N<=50

对于100%数据，2 ≦ N ≦ 100000, 1 ≦ M ≦ 200000, 0 ≦ K ≦ N - 2, 0 ≦ S ≦ 100000

1 ≦ P ＜ Q ≦ 100000


## 思路

计算出点权，按点权dijkstra即可

### 计算点权

* 普通城市 点权为p

* 危险城市 点权为q

* 被占领城市 不可达，无点权

* 起点和终点 点权为0

关键在于计算出哪些是危险城市，即被占领城市周围s层内的城市

并不需要对每一个被占领城市进行一次BFS。

所有被占领城市先进队列，用last记录最后一个进队列的顶点，tail记为上一层的最后一个顶点

若所有被占领城市为第0层，我们开始计算周围第一层，last和tail的值相同，为所有被占领城市中最后一个进队列的(last为最后进队列的顶点，tail为上一层即第0层最后一个顶点)，while队列不空则一个顶点出队列，这个顶点扩展到的顶点均为第一层的顶点，它们都进队列，更新last为它们中的最后一个。若当前出队列的顶点为tail，则第0层的都扩展完毕，准备开始将第一层的顶点出队列并扩展，更新tail为last。记录层数，层数到达s时跳出循环。


```
    6
    |
    |
    |
2---0---1---4---7---8---9
    |       |   |
    |       |   |
    |       |   |
    3       5   10
```

如图，若0和1被占领，s=2，则 2 3 4 5 6 7 为危险顶点


||last|tail|队列|危险顶点(将危险顶点的点权设为q)|层数|			
|---|---|---|---|---|---|		
|0，1进队列|1|1|0 1|-|0|
|0出队列，2，3，6进队列|6|1|1 2 3 6|2 3 6|0|
|1出队列，4进队列|4|1|2 3 6 4|2 3 4 6|0|
|发现1==tail，更新|4|4|2 3 6 4|2 3 4 6|1|
|2出队列|4|4|3 6 4|2 3 4 6|1|
|3出队列|4|4|6 4|2 3 4 6|1|
|6出队列|4|4|4|2 3 4 6|1|
|4出队列，5，7进队列|7|4|5 7|2 3 4 5 6 7|1|
|发现4==tail，更新|7|7|5 7|2 3 4 5 6 7|2|
|到达第2层，结束|||||||


##### 注意 BFS有可能扩展到起点和终点，即将起点和终点的点权置为q，但不论如何它们的点权应该为0，[参考](https://www.luogu.org/discuss/show/27571)

### 计算最短路

* 按点权计算，即(a, b)的边权为b的点权，(b, a)的边权为a的点权。

* 记录图时，不需要记录具体的边权，dijkstra时，假设对min_v扩展邻接点，邻接点设为adj_v，则min_v到adj_v的边权即adj_v的点权。

* 对于被占领的顶点不能被扩展

其余和普通dijkstra无区别。

## C++代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1
typedef long long LL;

class Escape {
public:
    Escape() {}
    ~Escape() {}

    //main中调用的函数
    void escape();
private:
    //dijkstra中优先队列使用的结构体
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

    //private成员函数
    void input(); //输入
    void bfs(); //计算危险城市
    LL dijkstra(); //计算最短路径
    void clear(); //释放内存

    //类变量
    int n, m, k, s;
    int p, q;
    int src, des;
    vector<int> *graph;
    int *cost_; //cost_[i]为城市i的费用，即点权
};

void Escape::escape() {
    input();
    bfs();
    printf("%lld", dijkstra());
    clear();
}

void Escape::input() {
    scanf("%d %d %d %d %d %d", &n, &m, &k, &s, &p, &q);
    src = 0, des = n - 1; //改为从0开始编号
    cost_ = new int[n];
    fill(cost_, cost_ + n, p);
    for (int i = 0, ci; i < k; i++) {
        scanf("%d", &ci);
        ci--; //改为从0开始编号
        cost_[ci] = NO_VALUE;
    }
    graph = new vector<int>[n];
    for (int i = 0, ai, bi; i < m; i++) {
        scanf("%d %d", &ai, &bi);
        ai--; bi--; //改为从0开始编号
        graph[ai].push_back(bi);
        graph[bi].push_back(ai);
    }
}

void Escape::bfs() {	
    //被占领城市周围s层内的城市为危险城市
    if (s <= 0) return;
    queue<int> que;
    bool *collected = new bool[n];
    fill(collected, collected + n, false);
    int last, tail; //last为当前最后一个进队列的，tail为上一层的最后一个
    for (int i = 0; i < n; i++) 
        if (cost_[i] == NO_VALUE) {
            //遍历所有被占领城市
            que.push(i);
            collected[i] = true;
            last = tail = i;
        }
    int level = 0; //当前记为0层
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (int adj_v : graph[v]) 
            if (!collected[adj_v]) {
                collected[adj_v] = true;				
                que.push(adj_v);
                cost_[adj_v] = q; //记为危险城市
                last = adj_v; //更新最后入队的顶点
            }
        if (v == tail) { //若上一层的最后一个出队列了，则这一层结束
            if (++level >= s) { //层数增加
                while (!que.empty()) que.pop();
                break;
            }
            tail = last; //更新tail为本层最后一个，即last
        }
    }
    free(collected);
    cost_[src] = cost_[des] = 0;
}

LL Escape::dijkstra() {
    //dijkstra计算最短路径
    //这里(a, b)的费用即为b的费用，(b, a)的费用即为a的费用，即
    //(a, b)的边权为b的点权，(b, a)的边权为a的点权
    LL *dist = new LL[n];
    fill(dist, dist + n, NO_VALUE);
    bool *collected = new bool[n];
    fill(collected, collected + n, false);
    dist[src] = 0;
    priority_queue<Node, vector<Node>, cmp> que;
    que.push(Node(src, dist[src]));
    int min_v;
    LL tmp_dist;
    while (!que.empty()) {
        min_v = que.top().v;
        que.pop();
        if (collected[min_v]) continue;
        collected[min_v] = true;
        if (min_v == des) {
            while (!que.empty()) que.pop();
            break;
        }
        for (int adj_v : graph[min_v]) { //遍历邻接点
            if (!collected[adj_v] && cost_[adj_v] != NO_VALUE) {
                tmp_dist = dist[min_v] + cost_[adj_v]; //(min_v, adj_v) 的边权为 adj_v的点权即cost_[adj_v]
                if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
                    dist[adj_v] = tmp_dist;
                    que.push(Node(adj_v, tmp_dist));
                }
            }
        } //for adj_v
    } //while
    LL result = dist[des];
    free(dist);
    free(collected);
    return result;
}

void Escape::clear() {
    free(cost_);
    for (int i = 0; i < n; i++)
        vector<int>().swap(graph[i]);
}

int main() {
    Escape escape;
    escape.escape();
    return 0;
}
```





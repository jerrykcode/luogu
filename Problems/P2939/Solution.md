# P2939 [USACO09FEB]改造路Revamping Trails
[原题地址](https://www.luogu.org/problemnew/show/P2939)



题意翻译
----

约翰一共有N)个牧场.由M条布满尘埃的小径连接.小径可 以双向通行.每天早上约翰从牧场1出发到牧场N去给奶牛检查身体.

通过每条小径都需要消耗一定的时间.约翰打算升级其中K条小径，使之成为高 速公路.在高速公路上的通行几乎是瞬间完成的，所以高速公路的通行时间为0.

请帮助约翰决定对哪些小径进行升级，使他每天从1号牧场到第N号牧场所花的时间最短

题目描述
----

Farmer John dutifully checks on the cows every day. He traverses some of the M (1 <= M <= 50,000) trails conveniently numbered 1..M from pasture 1 all the way out to pasture N (a journey which is always possible for trail maps given in the test data). The N (1 <= N <= 10,000) pastures conveniently numbered 1..N on Farmer John's farm are currently connected by bidirectional dirt trails. Each trail i connects pastures P1_i and P2_i (1 <= P1_i <= N; 1 <= P2_i <= N) and requires T_i (1 <= T_i <= 1,000,000) units of time to traverse.

He wants to revamp some of the trails on his farm to save time on his long journey. Specifically, he will choose K (1 <= K <= 20) trails to turn into highways, which will effectively reduce the trail's traversal time to 0. Help FJ decide which trails to revamp to minimize the resulting time of getting from pasture 1 to N.

TIME LIMIT: 2 seconds

输入输出格式
------

输入格式：

* Line 1: Three space-separated integers: N, M, and K

* Lines 2..M+1: Line i+1 describes trail i with three space-separated integers: P1_i, P2_i, and T_i

输出格式：

* Line 1: The length of the shortest path after revamping no more than K edges

输入输出样例
------

输入样例#1：

> 4 4 1  
> 1 2 10  
> 2 4 10  
> 1 3 1  
> 3 4 100  

输出样例#1：

> 1  

说明
--

K is 1; revamp trail 3->4 to take time 0 instead of 100. The new shortest path is 1->3->4, total traversal time now 1.

## 思路

分层图最短路

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef unsigned char Byte; //一字节类型
typedef long long LL; //long long
typedef unsigned short Vertex; //顶点

// 邻接点结构体
struct AdjNode {
	Vertex adj_v; //邻接点
	int adj_weight; //邻接边权重
	AdjNode(Vertex adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//Dijkstra类
class Dijkstra {
public:
	/* 分层图最短路。
		最多将k条边的权重减为0的情况下，计算源(0)到终点(nv - 1)的最短距离。*/
	LL dijkstra(vector<AdjNode> *graph, int nv, int k);
private:
	//优先队列使用的结构体
	struct PriorityNode {
		Vertex v;
		LL dist;
		Byte level; //层数
		PriorityNode(Vertex v, LL dist, Byte level) : v(v), dist(dist), level(level) {}
	};

	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};
};

LL Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int k) {	
	LL **dist = new LL*[k + 1]; //dist[i][j]表示在最多将i条边的权重减为0的情况下，0到j的最短距离
	bool **collected = new bool*[k + 1]; //collected[i][j]表示dist[i][j]是否已确定
	for (int i = 0; i < k + 1; i++) {
		dist[i] = new LL[nv];
		fill(dist[i], dist[i] + nv, NO_VALUE);
		collected[i] = new bool[nv];
		fill(collected[i], collected[i] + nv, false);
	}
	Vertex src = 0, des = nv - 1;
	dist[0][src] = 0;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
	q.push(PriorityNode(src, 0, 0));
	Vertex top_v, adj_v;
	Byte top_level;
	LL top_dist, tmp_dist, result = NO_VALUE;
	while (!q.empty()) {
		top_v = q.top().v;
		top_level = q.top().level;
		top_dist = q.top().dist;
		q.pop();
		if (collected[top_level][top_v]) continue;
		collected[top_level][top_v] = true;
		if (top_v == des) { //到达终点
			while (!q.empty()) q.pop();
			result = top_dist; //记录结果
			break;
		}
		for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
			adj_v = it->adj_v;
			if (!collected[top_level][adj_v]) {
				//若top_v到adj_v的边权不减小
				tmp_dist = top_dist + it->adj_weight;
				if (tmp_dist < dist[top_level][adj_v] || dist[top_level][adj_v] == NO_VALUE) {
					dist[top_level][adj_v] = tmp_dist;
					q.push(PriorityNode(adj_v, tmp_dist, top_level));
				}
			}
			if (top_level < k && !collected[top_level + 1][adj_v]) {
				//若top_v到adj_v的边权减小为0
				tmp_dist = top_dist;
				if (tmp_dist < dist[top_level + 1][adj_v] || dist[top_level + 1][adj_v] == NO_VALUE) { 
					//注意是下一层的dist
					dist[top_level + 1][adj_v] = tmp_dist;
					q.push(PriorityNode(adj_v, tmp_dist, top_level + 1));
				}
			}
		} //for
	} //while
	for (int i = 0; i < k + 1; i++) {
		free(dist[i]);
		free(collected[i]);
	}
	free(dist);
	free(collected);
	return result;
}

int main() {
	int n, m, k;
	scanf("%d %d %d", &n, &m, &k);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	for (int i = 0, p1, p2, t; i < m; i++) {
		scanf("%d %d %d", &p1, &p2, &t);
		p1--; p2--;
		graph[p1].push_back(AdjNode(p2, t));
		graph[p2].push_back(AdjNode(p1, t));
	}
	Dijkstra dijkstra;
	printf("%ld", dijkstra.dijkstra(graph, n, k));
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
```


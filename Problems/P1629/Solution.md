# P1629 邮递员送信

[原题地址](https://www.luogu.org/problemnew/show/P1629)

## 题目描述

有一个邮递员要送东西，邮局在节点1.他总共要送N-1样东西，其目的地分别是2~N。由于这个城市

的交通比较繁忙，因此所有的道路都是单行的，共有M条道路，通过每条道路需要一定的时间。这个

邮递员每次只能带一样东西。求送完这N-1样东西并且最终回到邮局最少需要多少时间。

## 输入输出格式

### 输入格式：

第一行包括两个整数N和M。

第2到第M+1行，每行三个数字U、V、W，表示从A到B有一条需要W时间的道路。 满足

1<=U,V<=N,1<=W<=10000,输入保证任意两点都能互相到达。

【数据规模】

* 对于30%的数据，有1<=N<=200;

* 对于100%的数据，有1<=N<=1000,1<=M<=100000。

### 输出格式：

输出仅一行，包含一个整数，为最少需要的时间。

## 输入输出样例

### 输入样例#1： 

> 5 10  
> 2 3 5  
> 1 5 5  
> 3 5 6  
> 1 2 8  
> 1 3 8  
> 5 3 4  
> 4 1 8  
> 4 5 3  
> 3 5 6  
> 5 4 2  

### 输出样例#1： 

> 83  


## 思路

堆优化dijkstra，计算源到每一个顶点最短路径之和。然后对原图的反向图进行一次dijkstra，计

算反向图中源到每个顶点的最短路径之和。`反向图中源到顶点的最短路径即原图中顶点到源的最短路径`。

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef long long LL;

//邻接点结构体
struct AdjNode {
	int v; //邻接顶点
	int weight; //邻接边权重
	AdjNode(int v, int weight) : v(v), weight(weight) {}
};

//计算最短路径和的类
class Dijkstra {
public:
	Dijkstra() {}
	~Dijkstra() {}

	/* 返回从0到所有顶点最短路径之和。*/
	LL dijkstra(vector<AdjNode>* graph, int n);
private:
	//优先队列使用的结构体
	struct Node {
		int v;
		int dist;
		Node(int v, int dist) : v(v), dist(dist) {}
	};

	struct cmp {
		bool operator() (Node& a, Node& b) {
			return a.dist > b.dist;
		}
	};

	//类变量
	int *dist_;
	bool *collected_;
};

LL Dijkstra::dijkstra(vector<AdjNode>* graph, int n) {
	LL result = 0;
	dist_ = new int[n];
	fill(dist_, dist_ + n, NO_VALUE);
	collected_ = new bool[n];
	fill(collected_, collected_ + n, false);
	int src = 0;
	dist_[src] = 0;
	priority_queue<Node, vector<Node>, cmp> q;
	q.push(Node(src, dist_[src]));
	while (!q.empty()) {
		int minV = q.top().v;
		q.pop();
		if (collected_[minV]) continue;
		collected_[minV] = true;
		result += dist_[minV]; //minV的最短路径已确定，加到result中
		for (vector<AdjNode>::iterator it = graph[minV].begin(); it != graph

[minV].end(); it++) {
			int adjVertex = it->v;
			int adjWeight = it->weight;
			if (!collected_[adjVertex])
				if (dist_[minV] + adjWeight < dist_[adjVertex] || 

dist_[adjVertex] == NO_VALUE) {
					dist_[adjVertex] = dist_[minV] + adjWeight;
					q.push(Node(adjVertex, dist_[adjVertex]));
				}
		}
	}
	free(dist_);
	free(collected_);
	return result;
}

void clearGraph(vector<AdjNode> *graph, int n) {
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int a, b, dist;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &a, &b, &dist);
		a--; b--;
		graph[a].push_back(AdjNode(b, dist));
	}
	Dijkstra dijkstra;
	LL result = dijkstra.dijkstra(graph, n);
	//对反向图进行dijkstra
	vector<AdjNode> *inverseGraph = new vector<AdjNode>[n];
	for (int v = 0; v < n; v++) 
		for (vector<AdjNode>::iterator it = graph[v].begin(); it != graph

[v].end(); it++) {
			inverseGraph[it->v].push_back(AdjNode(v, it->weight));
		}
	clearGraph(graph, n);
	result += dijkstra.dijkstra(inverseGraph, n);
	clearGraph(inverseGraph, n);
	printf("%d\n", result);
}
```
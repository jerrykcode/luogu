# P1186 玛丽卡
[原题地址](https://www.luogu.org/problemnew/show/P1186)

## 题目描述

麦克找了个新女朋友，玛丽卡对他非常恼火并伺机报复。
因为她和他们不住在同一个城市，因此她开始准备她的长途旅行。
在这个国家中每两个城市之间最多只有一条路相通，并且我们知道从一个城市到另一个城市路上所需花费的时间。
麦克在车中无意中听到有一条路正在维修，并且那儿正堵车，但没听清楚到底是哪一条路。无论哪一条路正在维修，从玛丽卡所在的城市都能到达麦克所在的城市。
玛丽卡将只从不堵车的路上通过，并且她将按最短路线行车。麦克希望知道在最糟糕的情况下玛丽卡到达他所在的城市需要多长时间，这样他就能保证他的女朋友离开该城市足够远。
编写程序，帮助麦克找出玛丽卡按最短路线通过不堵车道路到达他所在城市所需的最长时间(用分钟表示)。

## 输入输出格式

### 输入格式：

第一行有两个用空格隔开的数N和M，分别表示城市的数量以及城市间道路的数量。1<=N<=1000,1<=M<=N \times (N-1)/21<=N<=1000,1<=M<=N*(N?1)/2。城市用数字1~N标识，麦克在城市1中，玛丽卡在城市N中。
接下来的M行中每行包含三个用空格隔开的数A,B,V。其中1<=A,B<=N,1<=V<=1000。这些数字表示在A和城市B中间有一条双行道，并且在V分钟内是就能通过。

### 输出格式：

一行，写出用分钟表示的最长时间，在这段时间中，无论哪条路在堵车，玛丽卡应该能够到达麦克处，如果少于这个时间的话，则必定存在一条路，该条路一旦堵车，玛丽卡就不能够赶到麦克处。

## 输入输出样例

### 输入样例#1： 

> 5 7  
> 1 2 8  
> 1 4 10  
> 2 3 9  
> 2 4 10  
> 2 5 1  
> 3 4 7  
> 3 5 10  

### 输出样例#1： 

> 27  

## 思路

先用dijkstra计算最短距离并记录相应路径，依次删除路径中的一条边，再次计算最短距离(在删除下一条边时，先将之前删除的边加回图中)，输出所有距离中最大的。

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
	int v; //邻接顶点
	int weight; //邻接边权重
	bool inUse; //true则此边未被删除
	AdjNode(int v, int weight) : v(v), weight(weight), inUse(true) {}
};

//Dijkstra类
class Dijkstra {
public:
	Dijkstra() {}
	~Dijkstra() {}

	/* 计算从源(n - 1)到终点(0)的最短距离，若findPath为true则需要记录路径。*/
	int dijkstra(vector<AdjNode> *graph, int n, bool findPath);
	
	/* 得到路径。*/
	void getPath(int **pPath);

	/* 释放路径数组内存。*/
	void clearPath() { free(path_); }
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
	//dist_和collected_在dijkstra函数中申请的内存在函数结束前释放
	int *dist_;
	bool *collected_;

	//path_在dijkstra函数中申请的内存需要调用clearPath函数来释放
	int *path_;
};

int Dijkstra::dijkstra(vector<AdjNode>* graph, int n, bool findPath) {
	dist_ = new int[n];
	fill(dist_, dist_ + n, NO_VALUE);
	collected_ = new bool[n];
	fill(collected_, collected_ + n, false);
	if (findPath) {
		path_ = new int[n];
	}
	int src = n - 1, des = 0;
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
		for (AdjNode adjNode : graph[minV]) {
			if (!adjNode.inUse) continue; //该边被删除
			int adjVertex = adjNode.v;
			int adjWeight = adjNode.weight;
			if (!collected_[adjVertex]) {
				if (dist_[minV] + adjWeight < dist_[adjVertex] || dist_[adjVertex] == NO_VALUE) {
					dist_[adjVertex] = dist_[minV] + adjWeight;
					q.push(Node(adjVertex, dist_[adjVertex]));
					if (findPath) path_[adjVertex] = minV; //记录路径
				}
			}
		}
	} //while
	int result = dist_[des];
	free(dist_);
	free(collected_);
	return result;
}

void Dijkstra::getPath(int ** pPath) {
	*pPath = path_;
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
		graph[b].push_back(AdjNode(a, dist));
	}
	Dijkstra dijkstra;
	int result = dijkstra.dijkstra(graph, n, true);
	int *path;
	dijkstra.getPath(&path);
	for (int des = 0; des != n - 1; des = path[des]) {
		auto it = graph[path[des]].begin();
		for (; it != graph[path[des]].end(); it++)
			if (it->v == des) break;
		it->inUse = false; //删除边
		int minDist = dijkstra.dijkstra(graph, n, false);
		if (minDist > result) result = minDist;
		it->inUse = true; //将删除的边加回来
	}
	dijkstra.clearPath();
	printf("%d", result);
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
```
# P2169 正则表达式
[原题地址](https://www.luogu.org/problemnew/show/P2169)

## 题目背景

小Z童鞋一日意外的看到小X写了一个正则表达式的高级程序，这个正则表达式程序仅仅由字符“0”,“1”，“.”和“*”构成，但是他能够匹配出所有在OJ上都AC的程序的核心代码！小Z大为颇感好奇，于是他决定入侵小X的电脑上去获得这个正则表达式的高级程序。

## 题目描述

在Internet网络中的每台电脑并不是直接一对一连通的，而是某些电脑之间存在单向的网络连接，也就是说存在A到B的连接不一定存在B到A的连接，并且有些连接传输速度很快，有些则很慢，所以不同连接传输所花的时间是有大有小的。另外，如果存在A到B的连接的同时也存在B到A的连接的话，那么A和B实际上处于同一局域网内，可以通过本地传输，这样花费的传输时间为0。

现在小Z告诉你整个网络的构成情况，他希望知道从他的电脑（编号为1），到小X的电脑（编号为n）所需要的最短传输时间。


## 输入输出格式

### 输入格式：

第一行两个整数n, m， 表示有n台电脑，m个连接关系。

接下来m行，每行三个整数u,v,w；表示从电脑u到电脑v传输信息的时间为w。

### 输出格式：

输出文件仅一行为最短传输时间。

## 输入输出样例

### 输入样例#1： 
> 3 2  
> 1 2 1  
> 2 3 1  

### 输出样例#1： 

> 2  

### 输入样例#2： 

> 5 5  
> 1 2 1  
> 2 3 6  
> 3 4 1  
> 4 2 1  
> 3 5 2  

### 输出样例#2： 
> 3  


## 说明

* 对于40%的数据，1<=n<=1000, 1<=m<=10000

* 对于70%的数据，1<=n<=5000, 1<=m<=100000

* 对于100%的数据，1<=n<=200000, 1<=m<=1000000

## 思路

trajan求强连通分支，每个强连通分支内的顶点互相之间的距离为0，所以将强连通分支作为顶点，创建另一个图，对此图用dijkstra求从小Z的顶点所属的强连通分支到小X所属的强连通分支的最短距离。

## 代码

```cpp
/*
洛谷P2169
https://www.luogu.org/problemnew/show/P2169
*/

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef long long LL;

//邻接点结构体
struct AdjNode {
	int v; //邻接点顶点
	int weight; //邻接边权重
	AdjNode(int v, int weight) : v(v), weight(weight) {}
};

//图结构体
typedef struct Graph {

	vector<AdjNode> *list; //邻接表
	int nv; //顶点数

	/* 构造函数 */
	Graph(int nv) : nv(nv) {
		list = new vector<AdjNode>[nv];
	}

	/* 插入边 */
	void insertEdge(int v, int w, int weight) {
		list[v].push_back(AdjNode(w, weight));
	}

	/* 删除邻接表vector */
	void clear() {
		for (int i = 0; i < nv; i++)
			vector<AdjNode>().swap(list[i]);
	}
} *PGraph;

//Tarjan类
class Tarjan {
public:
	Tarjan() {}
	~Tarjan() {}

	/* 对输入的图指针pGraph计算强连通分支，用color标记每个顶点所属的连通分量。*/
	int tarjan(PGraph pGraph, int *color);
private:
	/* dfs。*/
	void dfs(PGraph pGraph, int v, int *color);

	//类变量
	int *dfn_;
	int *low_;
	stack<int> vstack_;
	int vcount_; //顶点dfs次序，从1开始计数
	int color_count_; //连通分量计数，从0开始
};

int Tarjan::tarjan(PGraph pGraph, int * color) {
	int nv = pGraph->nv;
	dfn_ = new int[nv];
	low_ = new int[nv];
	fill(dfn_, dfn_ + nv, 0);
	fill(color, color + nv, NO_VALUE);
	vcount_ = 0;
	color_count_ = 0;
	for (int i = 0; i < nv; i++)
		if (!dfn_[i]) {
			dfs(pGraph, i, color);
		}
	free(dfn_);
	free(low_);
	return color_count_;
}

void Tarjan::dfs(PGraph pGraph, int v, int * color) {
	dfn_[v] = low_[v] = ++vcount_;
	vstack_.push(v);
	for (AdjNode adjNode : pGraph->list[v]) {
		int adjVertex = adjNode.v;
		if (!dfn_[adjVertex]) {
			dfs(pGraph, adjVertex, color);
			if (low_[adjVertex] < low_[v]) low_[v] = low_[adjVertex];
		}
		else if (color[adjVertex] == NO_VALUE) {
			if (dfn_[adjVertex] < low_[v]) low_[v] = dfn_[adjVertex];
		}
	}
	if (dfn_[v] == low_[v]) {
		while (vstack_.top() != v) {
			color[vstack_.top()] = color_count_;
			vstack_.pop();
		}
		color[vstack_.top()] = color_count_++;
		vstack_.pop();
	}
}

//Dijkstra类
class Dijkstra {
public:
	Dijkstra() {}
	~Dijkstra() {}

	LL dijkstra(PGraph pGraph, int src, int des);
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
};

LL Dijkstra::dijkstra(PGraph pGraph, int src, int des) {
	int nv = pGraph->nv;
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
		for (AdjNode adjNode : pGraph->list[minV]) {
			int adjVertex = adjNode.v;
			int adjWeight = adjNode.weight;
			if (!collected_[adjVertex]) {
				if (dist_[minV] + adjWeight < dist_[adjVertex] || dist_[adjVertex] == NO_VALUE) {
					dist_[adjVertex] = dist_[minV] + adjWeight;
					q.push(Node(adjVertex, dist_[adjVertex]));
				}
			}
		}
	} //while
	LL result = dist_[des];
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	PGraph pGraph = new Graph(n);	
	for (int i = 0; i < m; i++) {
		int v, w, time;
		scanf("%d %d %d", &v, &w, &time);
		pGraph->insertEdge(v - 1, w - 1, time);
	}
	int *color = new int[n];
	Tarjan tarjan;
	int nComponents = tarjan.tarjan(pGraph, color);
	//根据强连通分量再创建一个图，顶点为连通分量
	PGraph pComGraph = new Graph(nComponents);
	int *edge = new int[nComponents];
	for (int i = 0; i < n; i++) {
		int v = color[i];
		fill(edge, edge + nComponents, NO_VALUE);
		for (AdjNode adjNode : pGraph->list[i])
			if (v != color[adjNode.v]) {
				//i与adjNode.v不在一个连通分量中
				int w = color[adjNode.v];
				if (edge[w] == NO_VALUE || edge[w] > adjNode.weight)
					edge[w] = adjNode.weight;
			}
		for (int j = 0; j < nComponents; j++)
			if (edge[j] != NO_VALUE)
				pComGraph->insertEdge(v, j, edge[j]);
	}
	pGraph->clear();
	Dijkstra dijkstra;
	LL result = dijkstra.dijkstra(pComGraph, color[0], color[n - 1]);
	printf("%d", result);
	pComGraph->clear();
	return 0;
}
```
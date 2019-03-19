# P1342 请柬

[原题地址](https://www.luogu.org/problemnew/show/P1342)

## 题目描述
在电(shou)视(ji)时代,没有多少人观看戏剧表演。Malidinesia古董喜剧演员意识到这一事实，他们想宣传剧院,尤其是古色古香的喜剧片。他们已经打印请帖和所有必要的信息和计划。许多学生被雇来分发这些请柬。每个学生志愿者被指定一个确切的公共汽车站，他或她将留在那里一整天,邀请人们参与。

这里的公交系统是非常特殊的:所有的线路都是单向的，连接两个站点。公共汽车离开起始点，到达目的地之后又空车返回起始点。学生每天早上从总部出发，乘公交车到一个预定的站点邀请乘客。每个站点都被安排了一名学生。在一天结束的时候,所有的学生都回到总部。现在需要知道的是，学生所需的公交费用的总和最小是多少。

## 输入输出格式

### 输入格式：

第1行有两个整数n、m(1<=n,m<=1000000)，n是站点的个数，m是线路的个数。

然后有m行，每行描述一个线路，包括3个整数，起始点，目的地和价格。

总部在第1个站点，价钱都是整数，且小于1000000000。

### 输出格式：

输出一行，表示最小费用。

## 输入输出样例

### 输入样例#1： 

> 4 6  
> 1 2 10  
> 2 1 60  
> 1 3 20  
> 3 4 10  
> 2 4 5  
> 4 1 50  

### 输出样例#1： 

> 210  


## 说明
【注意】

此题数据规模较大，需要使用较为高效的算法，此题不设小规模数据分数。

## 思路

堆优化dijkstra求总部到所有站点的最短路径，然后对反向图也求一次总部到所有站点的最短路径。


## 代码

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cstdio>
using namespace std;

#define NO_VALUE -1

typedef long long Cost; //费用
typedef int Vertex; //顶点

struct AdjNode { //邻接点结构体
	Vertex v; //邻接顶点
	int weight; //邻接边权
	AdjNode(Vertex v, int weight) : v(v), weight(weight) {}
};

//优先队列需要的结构体
struct Node {
	Vertex v;
	Cost dist;
	Node(Vertex v, Cost dist) : v(v), dist(dist) {}
};

struct cmp {
	bool operator() (Node& a, Node& b) {
		return a.dist > b.dist;
	}
};

Cost dijkstra(vector<AdjNode>* graph, int n, Vertex src) {
	Cost totalCost = 0;
	Cost *dist = new Cost[n];
	fill(dist, dist + n, NO_VALUE);
	bool *collected = new bool[n];
	fill(collected, collected + n, false);
	dist[src] = 0;
	priority_queue<Node, vector<Node>, cmp> q;
	q.push(Node(src, 0));
	while (!q.empty()) {
		Node minNode = q.top();
		Vertex minV = minNode.v;
		q.pop();
		if (collected[minV]) continue;
		collected[minV] = true;
		totalCost += dist[minV];
	//	for (AdjNode adjNode : graph[minV]) { //洛谷编译错误range-based ‘for’ loops only available with -std=c++11 or -std=gnu++11
	//		Vertex adjVertex = adjNode.v;
	//		Cost adjWeight = adjNode.weight;
		for (vector<AdjNode>::iterator it = graph[minV].begin(); it != graph[minV].end(); it++) { //auto也编译错误:(
			Vertex adjVertex = it->v;
			int adjWeight = it->weight;
			if (!collected[adjVertex])
				if (dist[minV] + adjWeight < dist[adjVertex] || dist[adjVertex] == NO_VALUE) {
					dist[adjVertex] = dist[minV] + adjWeight;
					q.push(Node(adjVertex, dist[adjVertex]));
				}
		}
	}//while
	free(dist);
	free(collected);
	return totalCost;
}

void clearGraph(vector<AdjNode> *graph, int n) {
	for (int i = 0; i < n; i++) {
		graph[i].clear();
		vector<AdjNode>().swap(graph[i]);
	}
	//free(graph);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int src, des, cost;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &src, &des, &cost);
		graph[src - 1].push_back(AdjNode(des - 1, cost));
	}
	Cost totalCost = dijkstra(graph, n, 0);
	//构建反向图
	vector<AdjNode> *inverseGraph = new vector<AdjNode>[n];
	for (Vertex v = 0; v < n; v++) 
		//for (AdjNode adjNode : graph[v]) {
		for (vector<AdjNode>::iterator it = graph[v].begin(); it != graph[v].end(); it++) {
			//inverseGraph[adjNode.v].push_back(AdjNode(v, adjNode.weight));
			inverseGraph[it->v].push_back(AdjNode(v, it->weight));
		}
	clearGraph(graph, n);
	totalCost += dijkstra(inverseGraph, n, 0);
	clearGraph(inverseGraph, n);
	printf("%lld\n", totalCost);
	return 0;
}
```
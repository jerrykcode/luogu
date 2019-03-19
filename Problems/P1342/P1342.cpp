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
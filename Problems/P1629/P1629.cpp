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
		for (vector<AdjNode>::iterator it = graph[minV].begin(); it != graph[minV].end(); it++) {
			int adjVertex = it->v;
			int adjWeight = it->weight;
			if (!collected_[adjVertex])
				if (dist_[minV] + adjWeight < dist_[adjVertex] || dist_[adjVertex] == NO_VALUE) {
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
		for (vector<AdjNode>::iterator it = graph[v].begin(); it != graph[v].end(); it++) {
			inverseGraph[it->v].push_back(AdjNode(v, it->weight));
		}
	clearGraph(graph, n);
	result += dijkstra.dijkstra(inverseGraph, n);
	clearGraph(inverseGraph, n);
	printf("%d\n", result);
}
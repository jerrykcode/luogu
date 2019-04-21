#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef unsigned short Vertex; //顶点

//邻接点结构体
struct AdjNode {
	Vertex adj_v; //邻接点
	int adj_weight; //邻接边权重
	AdjNode(Vertex adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//计算次短路的类
class SecondShortestPath {
public:
	/* 计算次短路。
		@param graph 图
		@param nv 顶点数
		@param src 源
		@param des 终点
		@return int 次短路长度
	*/
	int getSecondShortestPath(vector<AdjNode> * graph, int nv, int src, int des);
private:
	/* 计算源到每个顶点的最短距离。
		@param graph 图
		@param nv 顶点数
		@param src 源
		@param dist 存储最短距离的数组(传出)
		@return void
	*/
	void dijkstra(vector<AdjNode> * graph, int nv, int src, int *dist);

	/* A*计算次短路。
		@param graph 图
		@param nv 顶点数
		@param src 源
		@param des 终点
		@param h 估值数组
		 @return int 次短路长度
	*/
	int astar(vector<AdjNode>* graph, int nv, int src, int des, int *h);

	//优先队列使用的结构体
	//dijkstra优先队列使用
	struct PriorityNode {
		Vertex v;
		int dist;
		PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
	};

	//A*优先队列使用
	struct AStarNode : PriorityNode {
		//PriorityNode::dist 为f
		int g;
		AStarNode(Vertex v, int g, int f) : PriorityNode(v, f), g(g) {}
	};

	struct cmp {
		bool operator () (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};	
};

int SecondShortestPath::getSecondShortestPath(vector<AdjNode>* graph, int nv, int src, int des) {
	int *h = new int[nv];
	//dijkstra计算des到每个顶点的最短距离，作为h
	dijkstra(graph, nv, des, h);
	//计算次短路
	int result = astar(graph, nv, src, des, h);
	free(h);
	return result;
}

void SecondShortestPath::dijkstra(vector<AdjNode>* graph, int nv, int src, int *dist) {
	fill(dist, dist + nv, NO_VALUE);
	bool *collected = new bool[nv];
	fill(collected, collected + nv, false);
	dist[src] = 0;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
	q.push(PriorityNode(src, 0));
	Vertex min_v, adj_v;
	int adj_weight, tmp_dist;
	while (!q.empty()) {
		min_v = q.top().v;
		q.pop();
		if (collected[min_v]) continue;
		collected[min_v] = true;
		for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) { 
			//遍历邻接点
			adj_v = it->adj_v;
			if (!collected[adj_v]) {
				adj_weight = it->adj_weight;
				tmp_dist = dist[min_v] + adj_weight;
				if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
					dist[adj_v] = tmp_dist;
					q.push(PriorityNode(adj_v, tmp_dist));
				}
			}
		} //for
	} //while
	free(collected);
}

int SecondShortestPath::astar(vector<AdjNode>* graph, int nv, int src, int des, int *h) {
	priority_queue<AStarNode, vector<AStarNode>, cmp> q;
	q.push(AStarNode(src, 0, h[src]));
	Vertex min_v, adj_v;
	int min_g, tmp_g, count = 0;
	while (!q.empty()) {
		min_v = q.top().v;
		min_g = q.top().g;
		q.pop();
		if (min_v == des) {
			if (++count == 2) {
				//第二次弹出des
				while (!q.empty()) q.pop();
				return min_g;
			}
		}
		for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) {
			//遍历邻接点
			adj_v = it->adj_v;
			tmp_g = min_g + it->adj_weight;
			q.push(AStarNode(adj_v, tmp_g, tmp_g + h[adj_v]));
		}
	} //while
	return NO_VALUE;
}

int main() {
	int n, r;
	scanf("%d %d", &n, &r);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	for (int i = 0, a, b, d; i < r; i++) {
		scanf("%d %d %d", &a, &b, &d);
		a--; b--;
		graph[a].push_back(AdjNode(b, d));
		graph[b].push_back(AdjNode(a, d));
	}
	SecondShortestPath ssp;
	printf("%d", ssp.getSecondShortestPath(graph, n, 0, n - 1));
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
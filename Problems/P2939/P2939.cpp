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
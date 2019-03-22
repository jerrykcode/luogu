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
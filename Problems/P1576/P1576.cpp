#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

//邻接点结构体
struct AdjNode {
	int v; //邻接顶点
	double weight; //邻接边权重
	AdjNode(int v, double weight) : v(v), weight(weight) {}
};

//计算最小花费类
class MinCost {
public:
	MinCost() {}
	~MinCost() {}

	/* 返回src需要得到num，des至少需要多少花费。*/
	double minCost(vector<AdjNode> *graph, int nv, int src, int des, double num);
private:
	//优先队列使用的结构体
	struct Node {
		int v;
		double dist;
		Node(int v, double dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator() (Node& a, Node& b) {
			return a.dist > b.dist;
		}
	};

	//类变量
	double *dist_;
	bool *collected_;
};

double MinCost::minCost(vector<AdjNode>* graph, int nv, int src, int des, double num) {
	//dijkstra
	dist_ = new double[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	collected_ = new bool[nv];
	fill(collected_, collected_ + nv, false);
	dist_[src] = num;
	priority_queue<Node, vector<Node>, cmp> q;
	q.push(Node(src, dist_[src]));
	int min_vertex, adj_vertex;
	double adj_weight, tmp_dist;
	while (!q.empty()) {
		min_vertex = q.top().v;
		q.pop();
		if (collected_[min_vertex]) continue;
		collected_[min_vertex] = true;
		if (min_vertex == des) {
			while (!q.empty()) q.pop();
			break;
		}
		for (AdjNode adj_node : graph[min_vertex]) { //遍历邻接点
			adj_vertex = adj_node.v;
			adj_weight = adj_node.weight;
			if (!collected_[adj_vertex]) {
				tmp_dist = dist_[min_vertex] / adj_weight; //adj_vertex经过min_vertex到达src的花费
				if (tmp_dist < dist_[adj_vertex] || dist_[adj_vertex] == NO_VALUE) {
					//更新
					dist_[adj_vertex] = tmp_dist;
					q.push(Node(adj_vertex, dist_[adj_vertex]));
				}
			}
		}
	} //while
	double result = dist_[des];
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int x, y, z, a, b;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &x, &y, &z);
		//改为从0开始编号
		x--; y--;
		graph[x].push_back(AdjNode(y, ((double)(1.0) - (double)(0.01) * z)));
		graph[y].push_back(AdjNode(x, ((double)(1.0) - (double)(0.01) * z)));
	}
	scanf("%d %d", &a, &b);
	a--; b--;
	MinCost min_cost;
	printf("%.8lf", min_cost.minCost(graph, n, b, a, (double)(100.0)));
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
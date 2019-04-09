#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define NO_VALUE -1
typedef long long LL;

//邻接点结构体
struct AdjNode {
	int v; //邻接点顶点
	int weight; //邻接边权重
	AdjNode(int v, int weight) : v(v), weight(weight) {}
};

//Dijkstra类
class Dijkstra {
public:
	Dijkstra() {}
	~Dijkstra() {}

	/*
		计算src到des的最短路径。
		@param graph 图
		@param nv 顶点数
		@param src 起点(源)
		@param des 终点
		@param f 点权数组，f[i]为i的点权
		@param max_f 最短路径上的顶点不能超过的最大点权
		@param P_min_dist 指针，用于传出最短距离
		@return 若在点权不超过max_f的情况下，src可到达des则返回true，否则返回false
	*/
	bool dijkstra(vector<AdjNode> *graph, int nv, int src, int des, int *f, int max_f, int *p_min_dist);
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
	priority_queue<Node, vector<Node>, cmp> queue_;
};

bool Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int *f, int max_f, int * p_min_dist) {
	if (f[src] > max_f || f[des] > max_f) return false;
	dist_ = new LL[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	collected_ = new bool[nv];
	fill(collected_, collected_ + nv, false);
	dist_[src] = 0;
	queue_.push(Node(src, dist_[src]));
	int min_vertex, adj_vertex, adj_weight;
	LL tmp_dist;
	while (!queue_.empty()) {
		min_vertex = queue_.top().v;
		queue_.pop();
		if (collected_[min_vertex]) continue;
		collected_[min_vertex] = true;
		if (min_vertex == des) {
			while (!queue_.empty()) queue_.pop();
			break;
		}
		for (AdjNode adj_node : graph[min_vertex]) { //遍历邻接点
			adj_vertex = adj_node.v;
			adj_weight = adj_node.weight;
			if (!collected_[adj_vertex] && f[adj_vertex] <= max_f) {
				tmp_dist = dist_[min_vertex] + adj_weight;
				if (tmp_dist < dist_[adj_vertex] || dist_[adj_vertex] == NO_VALUE) {
					dist_[adj_vertex] = tmp_dist;
					queue_.push(Node(adj_vertex, dist_[adj_vertex]));
				}
			}
		}
	} //while
	*p_min_dist = dist_[des];
	free(dist_);
	bool result = collected_[des];
	free(collected_);
	return result;
}

/* 二分查找。*/
int binarySearch(int *arr, int n, int val) {
	int left = 0, right = n - 1, mid;
	while (left <= right) {
		mid = (left + right) / 2;
		if (arr[mid] == val) return mid;
		else if (arr[mid] < val) left = mid + 1;
		else right = mid - 1;
	}
	return NO_VALUE;
}

int main() {
	int n, m, u, v, s;
	scanf("%d %d %d %d %d", &n, &m, &u, &v, &s);	
	u--; v--; //改为从0开始编号
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int *f = new int[n];
	for (int i = 0; i < n; i++)
		scanf("%d", f + i); //输入点权
	int *f_sorted = new int[n];
	memcpy(f_sorted, f, n * sizeof(int));
	sort(f_sorted, f_sorted + n); //排序点权
	int a, b, c;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &a, &b, &c);
		a--; b--;
		graph[a].push_back(AdjNode(b, c));
		graph[b].push_back(AdjNode(a, c));
	}
	Dijkstra dijkstra;
	int min_dist;
	int left = max(binarySearch(f_sorted, n, f[u]), binarySearch(f_sorted, n, f[v])); //点权一定大于等于src和des的点权
	int right = n - 1, mid, last = NO_VALUE;
	//二分
	while (left <= right) {
		mid = (left + right) / 2;
		if (dijkstra.dijkstra(graph, n, u, v, f, f_sorted[mid], &min_dist) && min_dist < s) {
			last = mid;
			right = mid - 1;
		}
		else left = mid + 1;
	}
	last == NO_VALUE ? printf("-1") : printf("%d", f_sorted[last]);
	free(f);
	free(f_sorted);
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
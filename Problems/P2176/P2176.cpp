#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef unsigned char Vertex; //顶点

//邻接点结构体
struct AdjNode {
	Vertex v; //邻接顶点
	int weight; //邻接边权重
	AdjNode(Vertex v, int weight) : v(v), weight(weight) {}
};

//Dijkstra类
class Dijkstra {
public :
	Dijkstra() : pre_memory_alloc_(false) {}
	~Dijkstra() { if (pre_memory_alloc_) free(pre_); }

	/* 计算源(0)到终点(nv - 1)的最短路径，用p_min_dist指针传出。nv为顶点数量，record_path表示是否记录路径。*/
	bool dijkstra(vector<AdjNode> *graph, int nv, bool record_path, int *p_min_dist);

	/* 依次得到最后一次调用dijkstra并记录路径时，最短路径上的边，用p_from，p_to指针传出。
	   第一次调用本函数得到最短路径上的最后一条边，之后每次调用得到上次调用的前一条边。*/
	bool getMinPath(Vertex *p_from, Vertex *p_to);
private:
	//优先队列使用的结构体
	struct Node {
		Vertex v;
		int dist;
		Node(Vertex v, int dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator() (Node& a, Node& b) {
			return a.dist > b.dist;
		}
	};

	//类变量
	int nv_;
	Vertex src_, des_; //nv_, src_, des_ 在getMinPath中用于记录

	int *dist_;
	bool *collected_;
	Vertex *pre_;
	bool pre_memory_alloc_; //pre_数组分配了内存则为true
};

bool Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, bool record_path, int * p_min_dist) {	
	dist_ = new int[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	collected_ = new bool[nv];
	fill(collected_, collected_ + nv, false);
	Vertex src = 0, des = nv - 1;
	if (record_path) { //需要记录路径
		//更新nv_，src_，des_
		nv_ = nv;
		src_ = src;
		des_ = des;
		if (pre_memory_alloc_) free(pre_);
		pre_ = new Vertex[nv];
		pre_memory_alloc_ = true;
	}	
	dist_[(size_t)src] = 0;
	priority_queue<Node, vector<Node>, cmp> q;
	q.push(Node(src, dist_[(size_t)src]));
	while (!q.empty()) {
		Vertex min_vertex = q.top().v;
		q.pop();
		if (collected_[(size_t)min_vertex]) continue;
		collected_[(size_t)min_vertex] = true;
		if (min_vertex == des) {
			while (!q.empty()) q.pop();
			break;
		}
		int min_dist = dist_[(size_t)min_vertex];
		for (AdjNode adjNode : graph[min_vertex]) {
			Vertex adj_vertex = adjNode.v;
			int adj_weight = adjNode.weight;
			if (!collected_[(size_t)adj_vertex]) {
				if (min_dist + adj_weight < dist_[(size_t)adj_vertex] || dist_[(size_t)adj_vertex] == NO_VALUE) {
					dist_[(size_t)adj_vertex] = min_dist + adj_weight;
					q.push(Node(adj_vertex, dist_[(size_t)adj_vertex]));
					if (record_path) pre_[(size_t)adj_vertex] = min_vertex; //记录路径
				}
			}
		}
	}//while
	bool result = collected_[(size_t)des];
	*p_min_dist = dist_[(size_t)des];
	free(dist_);
	free(collected_);
	return result;
}

bool Dijkstra::getMinPath(Vertex * p_from, Vertex * p_to) {	
	if (des_ == src_) return false;
	*p_from = pre_[(size_t)des_];
	*p_to = des_;
	des_ = *p_from;
	return true;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int a_i, b_i, l_i;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &a_i, &b_i, &l_i);
		a_i--; b_i--;
		graph[a_i].push_back(AdjNode(b_i, l_i));
		graph[b_i].push_back(AdjNode(a_i, l_i));
	}
	Dijkstra dijkstra;
	int min_dist, tmp_dist, result = 0;
	dijkstra.dijkstra(graph, n, true, &min_dist);
	Vertex from, to;	
	while (dijkstra.getMinPath(&from, &to)) { //遍历最短路径上的所有边
		auto it = graph[(size_t)from].begin();
		for (; it != graph[(size_t)from].end(); it++)
			if (it->v == to) {
				it->weight *= 2; //增倍
				break;
			}
		if (dijkstra.dijkstra(graph, n, false, &tmp_dist))
			if (tmp_dist - min_dist > result) result = tmp_dist - min_dist;
		it->weight /= 2; //还原
	}
	printf("%d", result);
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
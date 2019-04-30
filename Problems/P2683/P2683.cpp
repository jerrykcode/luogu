#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char Vertex;
#define NO_VALUE -1

class Graph {
public:
	Graph(int nv);
	~Graph();
	void insert(Vertex v, Vertex w, int e);
	int dijkstra(Vertex src, Vertex des);

private:
	struct AdjNode {
		Vertex adj_v;
		int adj_weight;
		AdjNode(Vertex adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
	};
	struct PriorityNode {
		Vertex v;
		int dist;
		PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};

	int nv_;
	vector<AdjNode> *list_;

	int *dist_;
	bool *collected_;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
};

Graph::Graph(int nv) : nv_(nv) {
	list_ = new vector<AdjNode>[nv];
}

Graph::~Graph() {
	for (size_t i = 0; i < nv_; i++)
		vector<AdjNode>().swap(list_[i]);
}

void Graph::insert(Vertex v, Vertex w, int e) {
	list_[v].push_back(AdjNode(w, e));
	list_[w].push_back(AdjNode(v, e));
}

int Graph::dijkstra(Vertex src, Vertex des) {
	dist_ = new int[nv_];
	fill(dist_, dist_ + nv_, NO_VALUE);
	collected_ = new bool[nv_];
	fill(collected_, collected_ + nv_, false);
	dist_[src] = 0;
	q.push(PriorityNode(src, 0));
	Vertex top_v, adj_v;
	int tmp_dist;
	while (!q.empty()) {
		top_v = q.top().v;
		q.pop();
		if (collected_[top_v]) continue;
		collected_[top_v] = true;
		if (top_v == des) {
			while (!q.empty()) q.pop();
			break;
		}
		for (auto it = list_[top_v].begin(); it != list_[top_v].end(); it++) {
			adj_v = it->adj_v;
			if (!collected_[adj_v]) {
				tmp_dist = dist_[top_v] + it->adj_weight;
				if (tmp_dist < dist_[adj_v] || dist_[adj_v] == NO_VALUE) {
					dist_[adj_v] = tmp_dist;
					q.push(PriorityNode(adj_v, tmp_dist));
				}
			}
		} //for
	} //while
	int result = collected_[des] ? dist_[des] : -1;
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	Graph *graph = new Graph(n);
	int cmd, v, w, e;
	for (size_t i = 0; i < m; i++) {
		scanf("%d", &cmd);
		if (cmd == 0) {
			scanf("%d %d", &v, &w);
			v--; w--;
			printf("%d\n", graph->dijkstra(v, w));
		}
		else {
			scanf("%d %d %d", &v, &w, &e);
			v--; w--;
			graph->insert(v, w, e);
		}
	}
	return 0;
}
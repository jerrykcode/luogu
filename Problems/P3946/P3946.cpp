#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef long long LL;
struct AdjNode {
	int adj_v;
	int adj_weight;
	AdjNode(int adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

class Dijkstra {
public:
	bool dijkstra(vector<AdjNode> *graph, int nv, int src, int des, int q, LL * v_height, LL *p_shortest_dist);
private:
	struct PriorityNode {
		int v;
		LL dist;
		PriorityNode(int v, LL dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};

	LL * dist_;
	bool * collected_;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> pq_;
};

bool Dijkstra::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int q, LL * v_height, LL * p_shortest_dist) {
	dist_ = new LL[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	collected_ = new bool[nv];
	fill(collected_, collected_ + nv, false);
	dist_[src] = 0;
	pq_.push(PriorityNode(src, 0));
	int top_v, adj_v;
	LL tmp_dist;
	while (!pq_.empty()) {
		top_v = pq_.top().v;
		pq_.pop();
		if (collected_[top_v]) continue;
		collected_[top_v] = true;
		if (top_v == des) {
			while (!pq_.empty()) pq_.pop();
			break;
		}
		if (dist_[top_v] * q > v_height[top_v]) continue;
		for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) {
			adj_v = it->adj_v;
			if (!collected_[adj_v]) {
				tmp_dist = dist_[top_v] +  it->adj_weight;
				if (tmp_dist < dist_[adj_v] || dist_[adj_v] == NO_VALUE) {
					dist_[adj_v] = tmp_dist;
					pq_.push(PriorityNode(adj_v, tmp_dist));
				}
			}
		} //for
	} //while
	bool result = collected_[des];
	*p_shortest_dist = dist_[des];
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m, s, t, g, q;
	scanf("%d %d %d %d %d %d", &n, &m, &s, &t, &g, &q);
	s--; t--;
	LL * height = new LL[n];
	LL hi, li;
	for (size_t i = 0; i < n; i++) {
		scanf("%d %d", &hi, &li);
		height[i] = li - hi;
	}
	vector<AdjNode> * graph = new vector<AdjNode>[n];
	int u, v, wi;
	for (size_t i = 0; i < m; i++) {
		scanf("%d %d %d", &u, &v, &wi);
		u--; v--;
		graph[u].push_back(AdjNode(v, wi));
		graph[v].push_back(AdjNode(u, wi));
	}
	Dijkstra dijkstra;
	LL shortest_dist;
	if (dijkstra.dijkstra(graph, n, s, t, q, height, &shortest_dist) && shortest_dist <= g) {
		printf("%lld", shortest_dist);
	}
	else printf("wtnap wa kotori no oyatsu desu!");
	free(height);
	for (size_t i = 0; i < n; i++) {
		vector<AdjNode>().swap(graph[i]);
	}
	return 0;
}
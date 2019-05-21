#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char byte;
#define BYTE_MAX 0xff
#ifdef INT_MAX
#undef INT_MAX
#endif
#define INT_MAX 0x7fffffff

struct AdjNode {
	byte adj_v;
	int adj_weight;
	AdjNode(byte adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

class Dinic {
public:
	bool getMaxFlow(vector<AdjNode> * graph, int nv, int * p_max_flow);
private:
	bool bfs(vector<AdjNode> * graph, int nv);
	int dfs(vector<AdjNode> * graph, int nv, byte src, int min_flow);

	queue<byte> vqueue_;
	byte * dist_;
};

bool Dinic::getMaxFlow(vector<AdjNode>* graph, int nv, int * p_max_flow) {
	bool result = false;
	int min_flow;
	*p_max_flow = 0;
	dist_ = new byte[nv];
	while (bfs(graph, nv)) {
		result = true;
		while (min_flow = dfs(graph, nv, 0, INT_MAX)) {
			*p_max_flow += min_flow;
		}
	}
	free(dist_);
	return result;
}

bool Dinic::bfs(vector<AdjNode>* graph, int nv) {
	byte src = 0, des = nv - 1;
	fill(dist_, dist_ + nv, BYTE_MAX);
	dist_[src] = 0;
	vqueue_.push(src);
	byte front_v, adj_v;
	while (!vqueue_.empty()) {
		front_v = vqueue_.front();
		vqueue_.pop();
		for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) {
			if (it->adj_weight != 0) {
				adj_v = it->adj_v;
				if (dist_[adj_v] == BYTE_MAX) {
					dist_[adj_v] = dist_[front_v] + 1;
					vqueue_.push(adj_v);
				}
			}
		} //for
	} //while
	return dist_[des] != BYTE_MAX;
}

int Dinic::dfs(vector<AdjNode>* graph, int nv, byte src, int min_flow) {
	byte des = nv - 1;
	if (src == des) {
		return min_flow;
	}
	byte adj_v;
	bool flag;
	int temp_flow;
	for (auto it = graph[src].begin(); it != graph[src].end(); it++) {
		adj_v = it->adj_v;
		if (it->adj_weight == 0 || dist_[adj_v] != dist_[src] + 1) continue;
		if (temp_flow = dfs(graph, nv, adj_v, min(min_flow, it->adj_weight))) {
			min_flow = temp_flow;
			it->adj_weight -= min_flow;
			flag = true;
			for (auto it1 = graph[adj_v].begin(); it1 != graph[adj_v].end(); it1++)
				if (it1->adj_v == src) {
					flag = false;
					it1->adj_weight += min_flow;
					break;
				}
			if (flag) {
				graph[adj_v].push_back(AdjNode(src, min_flow));
			}
			return min_flow;
		}
	}
	return 0;
}

int main() {
	int n, m, x, a, b, c;
	scanf("%d %d %d", &n, &m, &x);
	vector<AdjNode> * graph = new vector<AdjNode>[n];
	for (size_t i = 0; i < m; i++) {
		scanf("%d %d %d", &a, &b, &c);
		a--; b--;
		graph[a].push_back(AdjNode(b, c));
	}
	Dinic dinic;
	int max_flow;
	if (dinic.getMaxFlow(graph, n, &max_flow)) {
		printf("%d ", max_flow);
		int time = x / max_flow;
		if (x % max_flow) time++;
		printf("%d", time);
	}
	else printf("Orz Ni Jinan Saint Cow!");
	for (size_t i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}

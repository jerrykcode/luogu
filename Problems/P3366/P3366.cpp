#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

//邻接点结构体
struct AdjNode {
	int v; //邻接点
	int weight; //邻接边权重
	AdjNode(int v, int weight) : v(v), weight(weight) {}
};

class Prim {
public:
	Prim() {}
	~Prim() {}

	/* 若图连通返回true，否则返回false，用pMstWeight传出最小生成树权重。*/
	bool prim(vector<AdjNode> *graph, int n, int *pMstWeight);
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
};

bool Prim::prim(vector<AdjNode>* graph, int n, int * pMstWeight) {
	int *dist = new int[n];
	fill(dist, dist + n, NO_VALUE);
	int src = 0;
	dist[src] = 0;
	priority_queue<Node, vector<Node>, cmp> q;
	q.push(Node(src, dist[src]));
	int vcount = 0;
	*pMstWeight = 0;
	while (!q.empty()) {
		int minV = q.top().v;
		q.pop();
		if (minV != src && !dist[minV]) continue;
		(*pMstWeight) += dist[minV];
		dist[minV] = 0;
		vcount++;
		for (AdjNode adjNode : graph[minV]) {
			int adjVertex = adjNode.v;
			int adjWeight = adjNode.weight;
			if (dist[adjVertex]) {
				if (adjWeight < dist[adjVertex] || dist[adjVertex] == NO_VALUE) {
					dist[adjVertex] = adjWeight;
					q.push(Node(adjVertex, dist[adjVertex]));
				}
			}
		}
	} //while
	free(dist);
	return vcount == n;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int x, y, z;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &x, &y, &z);
		x--; y--;
		graph[x].push_back(AdjNode(y, z));
		graph[y].push_back(AdjNode(x, z));
	}
	Prim prim;
	int mstWeight;
	if (prim.prim(graph, n, &mstWeight)) {
		printf("%d", mstWeight);
	}
	else printf("orz");
	for (int i = 0; i < n; i++) {
		vector<AdjNode>().swap(graph[i]);
	}
	return 0;
}
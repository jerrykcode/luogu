#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define maxn 200005
#define maxs 400005
#define maxk maxs
#define maxnn maxn + maxk

typedef long long ll;
#define INF 0x3f3f3f3f3f3f3f3f

struct AdjNode {
	int v;
	ll weight;
	AdjNode(int v, ll weight) : v(v), weight(weight) {}
};
vector<AdjNode> graph[maxnn];

struct Node {
	int v;
	ll dist;
	Node(int v, ll dist) : v(v), dist(dist) {}
};
struct cmp {
	bool operator() (Node& a, Node& b) {
		return a.dist > b.dist;
	}
};
priority_queue<Node, vector<Node>, cmp> q;
ll dist[maxnn];
bool collected[maxnn];

int main() {
	int n, k, s, t;
	ll w;
	scanf("%d %d", &n, &k);
	int vidx = n; //virtual node index
	for (int i = 0; i < k; i++) {
		scanf("%d", &s);
		if (s) {
			vidx++;
			for (int j = 0; j < s; j++) {
				scanf("%d %lld", &t, &w);
				graph[vidx].push_back(AdjNode(t, w));
				graph[t].push_back(AdjNode(vidx, w));
			}
		}
	}
	//dijkstra
	dist[1] = 0;
	for (int i = 2; i <= vidx; i++) dist[i] = INF;
	q.push(Node(1, 0));
	while (!q.empty()) {
		int minv = q.top().v;
		q.pop();
		if (collected[minv]) continue;
		collected[minv] = true;
		for (auto it = graph[minv].begin(); it != graph[minv].end(); it++) {
			int v = it->v;
			if (collected[v]) continue;
			ll weight = it->weight;
			if (dist[minv] + weight < dist[v]) {
				dist[v] = dist[minv] + weight;
				q.push(Node(v, dist[v]));
			}
		}
	}
	for (int i = 1; i <= n; i++) {
		if (i != 1) putchar(' ');
		printf("%lld", dist[i]);
	}
	exit(0);
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
#define N 55
#define V (N<<1)
#define E (N*N*2+4*N)
struct edge {
	int to, f, c, next;
} e[E];
int head[V], ne;
inline void init_graph() { ne = 2; memset(head, 0, sizeof(head)); }
int k;
inline int ae(int u, int v, int f, int c) {
	e[ne].to = v, e[ne].f = f, e[ne].c = k * c, e[ne].next = head[u];
	head[u] = ne;
	return ne++;
}
int inc[V][V];
bool inq[V];
int h[V], dis[V];
struct {
	int from;
	int eidx;
	int minf;
} pre[V];
inline void spfa(int s) {
	queue<int> q;
	q.push(s);
	inq[s] = true;
	memset(h, 0x3f, sizeof(h));
	h[s] = 0;
	while (!q.empty()) {
		int u = q.front(); q.pop();
		inq[u] = false;
		for (int i = head[u]; i; i = e[i].next) {
			if (!e[i].f) continue;
			int v = e[i].to;
			if (h[u] + e[i].c < h[v]) {
				h[v] = h[u] + e[i].c;
				if (!inq[v]) {
					q.push(v);
					inq[v] = true;
				}
			}
		}
	}
}
struct pnode { int v, d; pnode(int v, int d) : v(v), d(d) {} };
struct cmp { bool operator() (pnode& a, pnode& b) { return a.d > b.d; } };
priority_queue<pnode, vector<pnode>, cmp> pq;
inline int dijkstra(int s, int t) {
	memset(dis, 0x3f, sizeof(dis));
	dis[s] = 0;
	pre[s].minf = 0x3f3f3f3f;
	pq.push(pnode(s, 0));
	while (!pq.empty()) {
		int u = pq.top().v;
		if (pq.top().d != dis[u]) { pq.pop(); continue; }
		pq.pop();
		for (int i = head[u]; i; i = e[i].next) {
			if (e[i].f) {
				int v = e[i].to;
				if (dis[u] + e[i].c + h[u] - h[v] < dis[v]) {
					dis[v] = dis[u] + e[i].c + h[u] - h[v];
					pre[v].from = u;
					pre[v].eidx = i;
					pre[v].minf = min(e[i].f, pre[u].minf);
					pq.push(pnode(v, dis[v]));
				}
			}
		}
	}
	return dis[t] < 0x3f3f3f3f;
}
int mcmf(bool min_cost, int n) {
	init_graph();
	k = min_cost ? 1 : -1;
	int s = 0, t = n << 1 | 1;
	for (int i = 1; i <= n; i++) ae(s, i, 1, 0), ae(i, s, 0, 0);
	for (int i = n + 1; i <= n << 1; i++) ae(i, t, 1, 0), ae(t, i, 0, 0);
	for (int i = 1; i <= n; i++) for (int j = n + 1; j <= n << 1; j++) ae(i, j, 1, inc[i][j - n]), ae(j, i, 0, -1 * inc[i][j - n]);
	spfa(s);
	int mc = 0;
	while (dijkstra(s, t)) {
		for (int u = 0; u <= t; u++) h[u] += dis[u];
		for (int u = t; u != s; u = pre[u].from) {
			e[pre[u].eidx].f -= pre[t].minf;
			e[pre[u].eidx ^ 1].f += pre[t].minf;
		}
		mc += h[t] * pre[t].minf;
	}
	return mc;
}
int main() {
	int n; scanf("%d", &n);
	for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) scanf("%d", &inc[i][j]);
	printf("%d\n%d\n", mcmf(true, n), -1 * mcmf(false, n));
	exit(0);
}

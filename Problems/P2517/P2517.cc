#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;
#define N 50
#define V (N+2)
#define E (6*N)
struct {
	int to, f, c, next;
} e[E];
int hd[V], ecnt = 2;
inline void ae(int u, int v, int f, int c) {
	#define _e e[ecnt]
	_e.to = v, _e.f = f, _e.c = c, _e.next = hd[u];
	#undef _e
	hd[u] = ecnt++;
}
inline void af(int u, int v, int f, int c) {
	ae(u, v, f, c), ae(v, u, 0, -c);
}
#define S 0
#define MONTH(i) (i+1)
#define T (MONTH(N - 1) + 1)
int h[V], dis[V];
bool inq[V];
queue<int>q;
#define INF_BYTE 0x3f
#define INF 0x3f3f3f3f
struct pnode { int v, p; pnode(int v, int p) : v(v), p(p) {} };
struct cmp { bool operator() (pnode& a, pnode& b) { return a.p > b.p; } };
priority_queue<pnode, vector<pnode>, cmp> pq;
struct {
	int from, eidx, minf;
} pre[V];
inline bool dijkstra() {
	memset(dis, INF_BYTE, sizeof(dis));
	dis[S] = 0;
	pre[S].minf = INF;
	pq.push(pnode(S, 0));
	while (pq.size()) {
		int u = pq.top().v, d = pq.top().p; pq.pop();
		if (dis[u] != d) continue;
		for (int i = hd[u]; i; i = e[i].next) if (e[i].f) {
			int v = e[i].to;
			if (dis[u] + h[u] + e[i].c - h[v] < dis[v]) {
				dis[v] = dis[u] + h[u] + e[i].c - h[v];
				pre[v].from = u;
				pre[v].eidx = i;
				pre[v].minf = min(e[i].f, pre[u].minf);
				pq.push(pnode(v, dis[v]));
			}
		}
	}
	return dis[T] != INF;
}
inline int mcmf() {
	memset(h, INF_BYTE, sizeof(h));
	h[S] = 0;
	q.push(S), inq[S] = true;
	while (q.size()) {
		int u = q.front(); q.pop(); inq[u] = false;
		for (int i = hd[u]; i; i = e[i].next) if (e[i].f) {
			int v = e[i].to;
			if (h[u] + e[i].c < h[v]) {
				h[v] = h[u] + e[i].c;
				if (!inq[v]) q.push(v), inq[v] = true;
			}
		}
	}
	if (h[T] == INF) return INF;
	int mc = 0;
	while (dijkstra()) {
		for (int u = S; u <= T; u++) h[u] += dis[u];
		mc += h[T] * pre[T].minf;
		for (int u = T; u != S; u = pre[u].from) {
			e[pre[u].eidx].f -= pre[T].minf;
			e[pre[u].eidx ^ 1].f += pre[T].minf;
		}
	}
	return mc;
}
int main() {
	int n, m, s; cin>>n>>m>>s;
	for (int i = 0, u; i < n; i++) {
		scanf("%d", &u);
		af(MONTH(i), T, u, 0);
	}
	for (int i = 0, d; i < n; i++) {
		scanf("%d", &d);
		af(S, MONTH(i), INF, d);
	}
	for (int i = 0; i < n; i++) {
		if (i < n - 1) af(MONTH(i), MONTH(i + 1), s, m);
	}
	cout<<mcmf();
	exit(0);
}
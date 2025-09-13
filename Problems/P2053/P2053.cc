#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;
#define N 60 //customer
#define M 9  //server
#define V (1 + N + M*N + 1)
#define S 0
#define T (V - 1)
#define E ((N + N * M * N + M * N) << 1)
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
queue<int> q;
int h[V], dis[V];
bool inq[V];
#define INF_BYTE 0x3f
#define INF 0x3f3f3f3f
struct pnode {
	int v, p;
	pnode(int v, int p) : v(v), p(p) {}
};
struct cmp { bool operator()(pnode &a, pnode &b) { return a.p > b.p; } };
priority_queue<pnode, vector<pnode>, cmp> pq;
struct {
	int from, eidx, minf;
} pre[V];
inline bool dijkstra() {
	memset(dis, INF, sizeof(dis));
	dis[S] = 0;
	pq.push(pnode(S, 0));
	pre[S].minf = INF;
	while (pq.size()) {
		int u = pq.top().v;
		int d = pq.top().p;
		pq.pop();
		if (d != dis[u]) continue;
		for (int i = hd[u]; i; i = e[i].next) if (e[i].f) {
			int v = e[i].to;
			if (d + e[i].c + h[u] - h[v] < dis[v]) {
				dis[v] = d + e[i].c + h[u] - h[v];
				pre[v].from = u;
				pre[v].eidx = i;
				pre[v].minf = min(pre[u].minf, e[i].f);
				pq.push(pnode(v, dis[v]));
			}
		}
	}
	return dis[T] < INF;
}
inline int mcmf() {
	int mc = 0;
	q.push(S);
	inq[S] = true;
	memset(h, INF_BYTE, sizeof(h));
	h[S] = 0;
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
	if (h[T] == INF) { exit(1); }
	while (dijkstra()) {
		for (int i = S; i <= T; i++) h[i] += dis[i];
		mc += pre[T].minf * h[T];
		for (int v = T; v != S; v = pre[v].from) {
			e[pre[v].eidx].f -= pre[T].minf;
			e[pre[v].eidx ^ 1].f += pre[T].minf;
		}
	}
	return mc;
}
int main() {
	int m, n; cin>>m>>n;
	#define SERVER_IDX(server_id, back_cnt) (n + server_id * n + back_cnt)
	for (int i = 1, t; i <= n; i++) {
		af(S, i, 1, 0);
		for (int j = 0; j < m; j++) {
			scanf("%d", &t);
			for (int back_cnt = 1; back_cnt <= n; back_cnt++) {
				af(i, SERVER_IDX(j, back_cnt), 1, back_cnt * t);
			}
		}
	}
	for (int i = 0; i < m; i++) 
		for (int back_cnt = 1; back_cnt <= n; back_cnt++) af(SERVER_IDX(i, back_cnt), T, 1, 0);
	#undef SERVER_IDX
	printf("%.2f\n", mcmf() * 1.0 / n);
	exit(0);
}
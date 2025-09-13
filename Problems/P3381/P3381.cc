#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
int n, m, s, t;
typedef long long ll;
ll mc, mf;
#define V 5003
#define M 50004
#define E ((M<<1)+666)
struct {
	int to;
	int next;
	ll f, c;
} e[E];
int hd[V], ecnt = 2;
inline void ae(int u, int v, ll f, ll c) {
	#define _e e[ecnt]
	_e.to = v, _e.next = hd[u], _e.f = f, _e.c = c;
	#undef _e
	hd[u] = ecnt++;
}
inline void af(int u, int v, ll f, ll c) {
	ae(u, v, f, c), ae(v, u, 0, -1 * c);
}
ll h[V], dis[V];
bool inq[V];
struct pnode { int v; ll d; pnode(int v, ll d) : v(v), d(d) {} };
struct cmp { bool operator()(pnode& a, pnode& b) { return a.d > b.d; } };
priority_queue<pnode, vector<pnode>, cmp> pq;
struct {
	int from;
	int eidx;
	ll minf;
} pre[V];
inline bool dijkstra() {
	memset(dis, 0x3f, sizeof(dis));
	dis[s] = 0;
	pre[s].minf = 0x3f3f3f3f3f3f3f3f;
	pq.push(pnode(s, 0));
	while (pq.size()) {
		int u = pq.top().v;
		ll d = pq.top().d;
		pq.pop();
		if (d != dis[u]) continue;
		for (int i = hd[u]; i; i = e[i].next) {
			if (e[i].f) {
				int v = e[i].to;
				if (dis[u] + e[i].c + h[u] - h[v] < dis[v]) {
					dis[v] = dis[u] + e[i].c + h[u] - h[v];
					pre[v].from = u;
					pre[v].eidx = i;
					pre[v].minf = min(pre[u].minf, e[i].f);
					pq.push(pnode(v, dis[v]));
				}
			}
		}
	}
	return dis[t] != 0x3f3f3f3f3f3f3f3f;
}
inline void mcmf() {
	mc = mf = 0;
	queue<int> q;
	memset(h, 0x3f, sizeof(h));
	h[s] = 0;
	q.push(s);
	inq[s] = true;
	while (q.size()) {
		int u = q.front(); q.pop();
		inq[u] = false;
		for (int i = hd[u]; i; i = e[i].next) if (e[i].f) {
			int v = e[i].to;
			if (h[u] + e[i].c < h[v]) {
				h[v] = h[u] + e[i].c;
				if (!inq[v]) q.push(v), inq[v] = true;
			}
		}
	}
	if (h[t] == 0x3f3f3f3f3f3f3f3f) return;
	while (dijkstra()) {
		for (int i = 1; i <= n; i++) if (dis[i] != 0x3f3f3f3f3f3f3f3f) h[i] += dis[i];
		mf += pre[t].minf;
		mc += h[t] * pre[t].minf;
		for (int v = t; v != s; v = pre[v].from) {
			e[pre[v].eidx].f -= pre[t].minf;
			e[pre[v].eidx ^ 1].f += pre[t].minf;
		}
	}
}
int main() {
	cin>>n>>m>>s>>t;
	{ll f, c; for (int i = 0, u, v; i < m; i++) { 
		scanf("%d %d %lld %lld", &u, &v, &f, &c); af(u, v, f, c); 
	} }
	mcmf(); cout<<mf<<" "<<mc;
	exit(0);
}
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <stack>
#include <iostream>
using namespace std;
#define N 1200
#define M 120000
#define V (N+66)
#define E ((M+666)<<1)
typedef long long ll;
struct {
	int to, next;
	ll f;
} e[E];
int hd[V], ecnt = 2;
inline void ae(int u, int v, ll f) {
	#define _e e[ecnt]
	_e.to = v, _e.next = hd[u], _e.f = f;
	#undef _e
	hd[u] = ecnt++;
}
inline void af(int u, int v, ll f) {
	ae(u, v, f), ae(v, u, 0);
}
int n, m, s, t;
int ht[V], gap[V];
ll ex[V];
stack<int> active[V];
#define INF_BYTE 0x3f
#define INF 0x3f3f3f3f
inline ll hlpp() {
	queue<int> q;
	memset(ht, INF_BYTE, sizeof(ht));
	ht[t] = 0;
	q.push(t);
	while (q.size()) {
		int u = q.front(); q.pop();
		gap[ht[u]]++;
		for (int i = hd[u]; i; i = e[i].next) if (e[i ^ 1].f) {
			int v = e[i].to;
			if (ht[u] + 1 < ht[v]) {
				ht[v] = ht[u] + 1;
				q.push(v);
			}
		}
	}
	if (ht[s] == INF) return 0;
	ht[s] = n;
	int maxHt = 0;
	for (int i = hd[s]; i; i = e[i].next) if (e[i].f) {
		int v = e[i].to;
		if (ht[v] < INF) {
			ex[v] = e[i].f;
			ex[s] -= e[i].f;
			e[i ^ 1].f += e[i].f;
			e[i].f = 0;
			if (v != t) active[ht[v]].push(v);
			if (ht[v] > maxHt) maxHt = ht[v];
		}
	}
	while (true) {
		while (maxHt >= 0 && active[maxHt].empty()) maxHt--;
		if (maxHt < 0) break;
		int u = active[maxHt].top(); active[maxHt].pop();
		for (int i = hd[u]; i && ex[u]; i = e[i].next) if (e[i].f) {
			int v = e[i].to;
			if (ht[v] + 1 == ht[u]) {
				ll f = min(ex[u], e[i].f);
				ex[u] -= f;
				if (!ex[v] && v != t) {
					active[ht[v]].push(v);
				}
				ex[v] += f;
				e[i].f -= f;
				e[i ^ 1].f += f;
			}
		}
		if (ex[u]) {
			if (--gap[ht[u]] == 0) {
				for (int i = 0; i < n; i++) if (ht[i] > ht[u]) ht[i] = n;
			}
			ht[u] = n;
			for (int i = hd[u]; i; i = e[i].next) if (e[i].f) {
				int v = e[i].to;
				if (ht[v] < ht[u]) ht[u] = ht[v];
			}
			if (ht[u] < n) {
				ht[u]++;
				gap[ht[u]]++;
				active[ht[u]].push(u);
				if (ht[u] > maxHt) maxHt = ht[u];
			}
		}
	}
	return ex[t];
}
int main() {
	cin>>n>>m>>s>>t;
	{int _m = m, u, v, f; while (_m--) {
		scanf("%d %d %d", &u, &v, &f);
		af(u, v, f);
	}}
	cout<<hlpp();
	exit(0);
}
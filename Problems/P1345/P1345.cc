#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

#define MAXN 101
#define N (MAXN<<1)
#define INF 0x3fffffff

struct Adj {
	int v, c;
	Adj(int v, int c) : v(v), c(c) {}
};
vector<Adj> g[N];
queue<int> q;
int dis[N];
int s, t;

inline bool bfs() {
	memset(dis, 0xff, sizeof dis); //-1
	dis[s] = 0;
	q.push(s);
	while (!q.empty()) {
		int v = q.front(); q.pop();
		for (auto it = g[v].begin(); it != g[v].end(); it++) {
			int w = it->v;
			if (dis[w] == -1 && it->c > 0) {
				dis[w] = dis[v] + 1;
				q.push(w);
			}
		}
	}
	return dis[t] != -1;
}

inline int min(int a, int b) {
	return a < b ? a : b;
}

int dfs(int v, int pathflow) {
	if (v == t || pathflow == 0) return pathflow;
	int resflow = 0;
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
		int w = it->v;
		if (dis[w] == dis[v] + 1 && it->c > 0) {
			int curflow = dfs(w, min(pathflow, it->c));
			it->c -= curflow;
			for (auto iw = g[w].begin(); iw != g[w].end(); iw++)
				if (iw->v == v) {
					iw->c += curflow;
					break;
				} // 反向边增流
			resflow += curflow;
			pathflow -= curflow;
			if (pathflow == 0) break;
		}
	}
	return resflow;
}

int dinic() {
	int res = 0;
	while (bfs()) {
		res += dfs(s, INF);
	}
	return res;
}

int main() {
	int n, m, v, w;
	scanf("%d %d %d %d", &n, &m, &s, &t);
	s--; t--;
	s += n;
	for (int i = 0; i < n; i++) {
		g[i].push_back(Adj(i + n, 1)); // 拆点
	}
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &v, &w);
		v--; w--;
		g[v + n].push_back(Adj(w, INF));
		g[w + n].push_back(Adj(v, INF));
	}
	printf("%d\n", dinic());
	exit(0);
}

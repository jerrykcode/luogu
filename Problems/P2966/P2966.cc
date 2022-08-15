#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdbool>
#include <queue>
#include <vector>
using namespace std;

#define N 251
int g[N][N];
int c[N];
int dist[N];
int ans[N][N];
bool collected[N];

struct PNode {
    int v, dist;
    PNode(int v, int dist) : v(v), dist(dist) {}
};

struct cmp {
    bool operator()(PNode& a, PNode& b) {
        return a.dist > b.dist;
    }
};

priority_queue<PNode, vector<PNode>, cmp> q;

inline void dijkstra(int s, int n) {
    int v, w;
    memset(dist, 0xff, sizeof(dist)); //-1
    memset(collected, 0, sizeof(collected)); // false
    dist[s] = 0;
    q.push(PNode(s, 0));
    while (!q.empty()) {
        v = q.top().v;
        q.pop();
        if (collected[v]) continue;
        collected[v] = true;
        for (w = 0; w < n; w++)
            if (g[v][w] != -1 && c[w] <= c[s] && (dist[v] + g[v][w] < dist[w] || dist[w] == -1)) {
                dist[w] = dist[v] + g[v][w];
                q.push(PNode(w, dist[w]));
            }
    }
}

int main() {
    memset(g, 0xff, sizeof(g)); //-1
    memset(ans, 0xff, sizeof(ans)); //-1
    int n, m, k, a, b, l;
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < n; i++) {
        scanf("%d", c + i);
    }
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a, &b, &l);
        a--, b--;
        if (l < g[a][b] || g[a][b] == -1) {
            g[a][b] = g[b][a] = l;
        }
    }
    for (int i = 0; i < n; i++) {
        dijkstra(i, n);
        for (a = 0; a < n; a++)
            for (b = a + 1; b < n; b++)
                if (dist[a] != -1 && dist[b] != -1 && (dist[a] + dist[b] + c[i] < ans[a][b] || ans[a][b] == -1))
                    ans[a][b] = ans[b][a] = dist[a] + dist[b] + c[i];
    }
    for (int i = 0; i < k; i++) {
        scanf("%d %d", &a, &b);
        a--, b--;
        printf("%d\n", ans[a][b]);
    }
    priority_queue<PNode, vector<PNode>, cmp>().swap(q);
    exit(0);
}

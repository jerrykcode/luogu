#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
using std::vector;
using std::priority_queue;

#define N 2500

struct AdjNode {
    int adjv;
    int weight;
    AdjNode(int adjv, int weight) : adjv(adjv), weight(weight) {}
};

vector<AdjNode> graph[N];

int dist[N];
bool collected[N];

struct PNode{
    int v;
    int dist;
    PNode(int v, int dist) : v(v), dist(dist) {}
};

struct cmp {
    bool operator()(PNode& a, PNode& b) {
        return a.dist > b.dist;
    }
};

priority_queue<PNode, vector<PNode>, cmp> q;

inline int dijkstra(int s, int t) {
    memset(dist, 0xff, sizeof(dist)); //-1
    memset(collected, 0, sizeof(collected)); //false
    dist[s] = 0;
    q.push(PNode(s, 0));
    while (!q.empty()) {
        int v = q.top().v;
        q.pop();
        if (collected[v]) continue;
        collected[v] = true;
        if (v == t) break;
        for (auto it = graph[v].begin(); it != graph[v].end(); it++) {
            int w = it->adjv;
            if (dist[w] == -1 || dist[v] + it->weight < dist[w]) {
                dist[w] = dist[v] + it->weight;
                q.push(PNode(w, dist[w]));
            }
        }
    }
    return dist[t];
}

int main() {
    int n, m, s, t, u, v, w;
    scanf("%d %d %d %d", &n, &m, &s, &t);
    s--; t--;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        u--; v--;
        graph[v].push_back(AdjNode(u, w));
        graph[u].push_back(AdjNode(v, w));
    }
    printf("%d", dijkstra(s, t));
    exit(0);
}

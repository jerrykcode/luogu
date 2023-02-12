#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

class AdjNode {
    friend class MST;
    int v, weight;
    AdjNode(int v, int weight): v(v), weight(weight) {}
};

class PNode {
    friend class MST;
    friend class Cmp;
    int v, dist;
    PNode(int v, int dist) : v(v), dist(dist) {}
};

class Cmp {
public:
    bool operator() (PNode& a, PNode& b) {
        return a.dist > b.dist;
    }
};

#define N 5000

class MST {
    private:
        int nv;
        int dist[N];
        vector<AdjNode> g[N];
        priority_queue<PNode, vector<PNode>, Cmp> q;
    public:
        MST(int nv) : nv(nv) {}
        void addEdge(int v, int w, int z) {
            v--, w--;
            g[v].push_back(AdjNode(w, z));
            g[w].push_back(AdjNode(v, z));
        }
        auto prim(int *mstWeight) -> bool;
};

int main() {
    int n, m, v, w, z;
    scanf("%d %d", &n, &m);
    MST *mst = new MST(n);
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &v, &w, &z);
        mst->addEdge(v, w, z);
    }
    int ans;
    if (mst->prim(&ans)) {
        printf("%d", ans);
    } else {
        printf("orz");
    }
    exit(0);
}

auto MST::prim(int *mstWeight) -> bool {
    *mstWeight = 0;
    memset(dist, 0xff, sizeof(dist));  // -1
    int s = 0, count = 0, v, d, w;
    dist[s] = 0;
    q.push(PNode(s, 0));
    while (!q.empty()) {
        v = q.top().v;
        d = q.top().dist;
        q.pop();
        if (dist[v] != d) continue;
        count++;
        *mstWeight += dist[v];
        dist[v] = 0;
        for (auto it = g[v].begin(); it != g[v].end(); it++) {
            w = it->v, d = it->weight;
            if (d < dist[w] || dist[w] == -1) {
                dist[w] = d;
                q.push(PNode(w, d));
            }
        }
    }
    return count == nv;
}

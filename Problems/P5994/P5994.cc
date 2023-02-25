#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

class AdjNode {
    friend class Prim;
    int v, weight;
public:
    AdjNode(int v, int weight) : v(v), weight(weight) {}
};

class PNode {
    friend class CmpNode;
    friend class Prim;
    int v, dist;
public:
    PNode(int v, int dist) : v(v), dist(dist) {}
};

class CmpNode {
public:
    auto operator() (PNode &a, PNode &b) -> bool {
        return a.dist > b.dist;
    }
};

class Prim {
public:
    ~Prim();
    auto mst(vector<AdjNode> *graph, int nv, long long *mst_weight) -> bool;
private:
    int *dist;
    priority_queue<PNode, vector<PNode>, CmpNode> q;
};

Prim::~Prim() { 
    if (dist) free(dist);
    priority_queue<PNode, vector<PNode>, CmpNode>().swap(q);
}

auto Prim::mst(vector<AdjNode> *graph, int nv, long long *mst_weight) -> bool {
    dist = reinterpret_cast<int *>(realloc(dist, nv*sizeof(int)));
    memset(dist, 0xff, nv*sizeof(int)); // -1
    int s = 0;
    dist[s] = 0;
    q.push(PNode(s, 0));
    *mst_weight = 0;
    int v, d, w;
    while (!q.empty()) {
        v = q.top().v;
        d = q.top().dist;
        q.pop();
        if (d != dist[v]) continue;
        nv--;
        dist[v] = 0;
        *mst_weight += d;
        for (auto it = graph[v].begin(); it != graph[v].end(); it++) {
            w = it->v;
            d = it->weight;
            if (d < dist[w] || dist[w] == -1) {
                dist[w] = d;
                q.push(PNode(w, d));
            }
        }
    }
    return nv == 0;
}

#define N 2003

vector<AdjNode> graph[N];

int main() {
    int n, i, j, c;
    cin >> n;
    for (i = 1; i <= n; i++)
        for (j = i; j <= n; j++) {
            scanf("%d", &c);
            graph[i - 1].push_back(AdjNode(j, c));
            graph[j].push_back(AdjNode(i - 1, c));
        }
    long long ans;
    Prim *prim = new Prim();
    if (prim->mst(graph, n + 1, &ans)) {
        cout << ans;
    }
    delete prim;
    exit(0);
}

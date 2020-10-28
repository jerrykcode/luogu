#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#define maxn 500000

typedef long long ll;

struct AdjNode {
    int v;
    int weight;
    AdjNode(int v, int weight) : v(v), weight(weight) {}
};

vector<AdjNode> tree[maxn];

ll len[maxn];
ll Dfs(int v, int parent) {
    ll numq = 0;
    for (auto it = tree[v].begin(); it != tree[v].end(); it++)
        if (it->v != parent) {            
            numq += Dfs(it->v, v);
            if (len[it->v] + it->weight > len[v]) len[v] = len[it->v] + it->weight;
        }
    for (auto it = tree[v].begin(); it != tree[v].end(); it++)
        if (it->v != parent) {
            numq += len[v] - len[it->v] - it->weight;
        }
    return numq;
}

int main() {
    int n, s, a, b, t;
    scanf("%d %d", &n, &s);
    s--;
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d %d", &a, &b, &t);
        a--; b--;
        tree[a].push_back(AdjNode(b, t));
        tree[b].push_back(AdjNode(a, t));
    }
    printf("%lld\n", Dfs(s, -1));
    exit(0);
}

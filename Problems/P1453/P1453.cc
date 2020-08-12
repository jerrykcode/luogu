#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#define maxn 100000

short p[maxn];
vector<int> graph[maxn];
int uset[maxn];
int dp[maxn][2];

void InitSet(int n) {
    for (int i = 0; i < n; i++)
        uset[i] = -1;
}

int FindRoot(int a) {
    if (uset[a] < 0) return a;
    return (uset[a] = FindRoot(uset[a]));
}

bool UnionSet(int a, int b) {
    int root_a = FindRoot(a), root_b = FindRoot(b);
    if (root_a == root_b) return false;
    if (uset[root_a] < uset[root_b]) {
        uset[root_a] += uset[root_b];
        uset[root_b] = root_a;
    }
    else {
        uset[root_b] += uset[root_a];
        uset[root_a] = root_b;
    }
    return true;
}

void AddEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

int max(int a, int b) {
    return a > b ? a : b;
}

void DP(int v, int parent) {
    dp[v][0] = 0;
    dp[v][1] = p[v];
    for (int u : graph[v]) {
        if (u == parent) continue;
        DP(u, v);
        dp[v][1] += dp[u][0];
        dp[v][0] += max(dp[u][0], dp[u][1]);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    InitSet(n);
    for (int i = 0; i < n; i++)
        scanf("%hd", &p[i]);
    int u, v;
    bool flag = true;
    int root_1, root_2;
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &u, &v);
        if (flag) {
            if (UnionSet(u, v)) {
                AddEdge(u, v);
            }
            else {
                flag = false;
                root_1 = u; root_2 = v;
            }
        }
        else {
            AddEdge(u, v);
        }
    }
    double k;
    scanf("%lf", &k);
    DP(root_1, -1); DP(root_2, -1);
    printf("%0.1lf\n", max(dp[root_1][0], dp[root_2][0]) * k);
    exit(0);
}

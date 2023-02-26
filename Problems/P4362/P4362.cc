#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

struct AdjNode {
    int v;
    int weight;
    AdjNode(int v, int weight) : v(v), weight(weight) {}
};

int m, k;
#define N 305
vector<AdjNode> tree[N];
int dp[N][N][2], tmp[N][2];

inline int min(int a, int b) { return a < b ? a : b; }
inline int min3(int a, int b, int c) { return min(a, min(b, c)); }

void dfs(int v, int fa) {
    dp[v][0][0] = dp[v][1][1] = 0;
    for (auto it = tree[v].begin(); it != tree[v].end(); it++) {
        int w = it->v, weight = it->weight;
        if (w == fa) continue;
        dfs(w, v);
        memcpy(tmp, dp[v], sizeof(tmp));
        memset(dp[v], 0x3f, sizeof(dp[v]));
        for (int j = 0; j <= k; j++)
            for (int t = 0; t <= j; t++) {
                dp[v][j][0] = min3(dp[v][j][0], dp[w][t][0] + tmp[j - t][0] + (m == 2) * weight, dp[w][t][1] + tmp[j - t][0]);
                dp[v][j][1] = min3(dp[v][j][1], dp[w][t][1] + tmp[j - t][1] + weight, dp[w][t][0] + tmp[j - t][1]);
            }
    }
}

int main() {
    int n, a, b, c;
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 1; i < n; i++) {
        scanf("%d %d %d", &a, &b, &c);
        tree[a].push_back(AdjNode(b, c));
        tree[b].push_back(AdjNode(a, c));
    }
    if (n - k < m - 1) { printf("-1"); exit(0); }
    memset(dp, 0x3f, sizeof(dp));
    dfs(1, 1);
    printf("%d", dp[1][k][1]);
    exit(0);
}

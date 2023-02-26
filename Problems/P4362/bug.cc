#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

#define N 303

struct Node {
    int v;
    int weight;
    Node(int v, int weight) : v(v), weight(weight) {}
};

vector<Node> tree[N];

int dp[N][N][2];
int tmp[N][2];

int M, K;

inline int min(int a, int b) { return a < b ? a : b; }

inline int min3(int a, int b, int c) { return min(a, min(b, c)); }

void dfs(int v, int fa) {
    dp[v][0][0] = dp[v][1][1] = 0;
    for (auto it = tree[v].begin(); it != tree[v].end(); it++) {
        int w = it->v, weight = it->weight;
        if (w == fa) continue;
        memcpy(tmp, dp[v], sizeof(dp[v]));
        memset(dp[v], 0x7f, sizeof(dp[v]));
        for (int k = 0; k <= K; k++)
            for (int t = 0; t <= k; t++) {
                dp[v][k][0] = min3(dp[v][k][0], dp[w][t][0] + tmp[k - t][0] + (M == 2) * weight,
                        dp[w][t][1] + tmp[k - t][0]);
                dp[v][k][1] = min3(dp[v][k][1], dp[w][t][0] + tmp[k - t][1],
                        dp[w][t][1] + tmp[k - t][1] + weight);
            }
    }
}

int main() {
    int n, a, b, c;
    scanf("%d %d %d", &n, &M, &K);
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d %d", &a, &b, &c);
        tree[a].push_back(Node(b, c));
        tree[b].push_back(Node(a, c));
    }
    if (n - K < M - 1) { printf("-1"); exit(0); }
    dfs(1, 1);
    printf("%d", dp[1][K][1]);
    exit(0);
}

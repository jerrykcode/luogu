#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#define maxn 16000
int dp[maxn];
int v[maxn];
vector<int> tree[maxn];

void DP(int u, int parent) {
    dp[u] = v[u];
    for (auto it = tree[u].begin(); it != tree[u].end(); it++)
        if (*it != parent) {
            DP(*it, u);
            if (dp[*it] > 0) dp[u] += dp[*it];
        }
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);
    int a, b;
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d", &a, &b);
        a--; b--;
        tree[a].push_back(b);
        tree[b].push_back(a);
    }
    DP(0, -1);
    int ans = dp[0];
    for (int i = 1; i < n; i++) {
        if (dp[i] > ans) ans = dp[i];
    }
    printf("%d\n", ans);
    return 0;
}

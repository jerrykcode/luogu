#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using std::vector;
using std::max;

#define maxn 6000

int r[maxn], dp[maxn][2];
vector<int> tree[maxn];
bool hasParent[maxn];

void dfs(int t);

int main() {
    int n, l, k;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", r + i);
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d", &l, &k);
        l--; k--;
        tree[k].push_back(l);
        hasParent[l] = true;
    }
    int root = 0;
    for (; root < n; root++)
        if (!hasParent[root]) break;
    dfs(root);
    printf("%d", max(dp[root][0], dp[root][1]));
    return 0;
}

void dfs(int t) {
    int a = 0, b = 0;
    for (auto it = tree[t].begin(); it != tree[t].end(); it++) {
        dfs(*it);
        a += dp[*it][0];
        b += max(dp[*it][0], dp[*it][1]);
    }
    dp[t][1] = a + r[t];
    dp[t][0] = b;
}

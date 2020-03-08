#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using std::vector;
using std::max;

#define maxn 301
#define maxm 302

vector<int> tree[maxn];
int dp[maxn][maxm];

int n, m;

void dfs(int r) {
    int s;
    for (auto it = tree[r].begin(); it != tree[r].end(); it++) {
        s = *it;
        dfs(s);
        for (int i = m; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                dp[r][i] = max(dp[r][i], dp[r][i - j] + dp[s][j]);
            }
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);
    m++;
    int p, s;
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &p, &s);
        tree[p].push_back(i);
//        for (int j = 1; j <= m; j++)
            dp[i][1] = s; //init
    }
    dfs(0);
    printf("%d", dp[0][m]);
    return 0;
}

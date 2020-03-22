#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#define maxn 100000

vector<int> tree[maxn];
int f[maxn];
int k, cnt;

void dfs(int t, int p) {
    f[t] = 1;
    for (int i = 0; i < tree[t].size(); i++) {
        if (tree[t][i] == p) continue;
        dfs(tree[t][i], t);
        f[t] += f[tree[t][i]];
    }
    if (f[t] == k) {
        cnt++;
        f[t] = 0;
    }
}

int main() {
    int t, n, a, b;
    scanf("%d", &t);
    while (t) {
        t--;
        scanf("%d %d", &n, &k);        
        if (n % k) {
            printf("NO\n");
            for (int i = 0; i < n - 1; i++) scanf("%d %d", &a, &b);
            continue;
        }
        for (int i = 0; i < n; i++) tree[i].clear();
        for (int i = 0; i < n - 1; i++) {
            scanf("%d %d", &a, &b);
            a--;b--;
            tree[a].push_back(b);
            tree[b].push_back(a);
        }
        cnt = 0;
        dfs(0, 0);
        if (cnt == n/k) printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}

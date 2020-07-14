#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
#include <set>
using namespace std;

#define maxn 10001

int v_weight[maxn];
vector<int> graph[maxn];
int dfn[maxn], low[maxn], dfs_cnt;
bool in_stack[maxn];
stack<int> stk;
int color[maxn], color_cnt;
int c_weight[maxn];
set<int> c_graph[maxn];
int dp[maxn];

void tarjan_dfs(int v) {
    dfn[v] = low[v] = ++dfs_cnt;
    stk.push(v);
    in_stack[v] = true;
    for (int w : graph[v]) {
        if (!dfn[w]) {
            tarjan_dfs(w);
            if (low[w] < low[v]) low[v] = low[w];
        }
        else if (in_stack[w]) {
            if (dfn[w] < low[v]) low[v] = dfn[w];
        }
    }
    if (dfn[v] == low[v]) {
        int w;
        do {
            w = stk.top(); stk.pop();
            in_stack[w] = false;
            color[w] = color_cnt;
            c_weight[color_cnt] += v_weight[w];            
        } while (w != v);
        color_cnt++;
    }
}

int dfs(int cv) {
    for (auto it = c_graph[cv].begin(); it != c_graph[cv].end(); it++) {
        if (!dp[*it]) dp[*it] = dfs(*it);
        if (dp[*it] > dp[cv]) dp[cv] = dp[*it];
    }
    dp[cv] += c_weight[cv];
    return dp[cv];
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%d", &v_weight[i]);
    }
    int u, v;
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &u, &v);
        u--; v--;
        graph[u].push_back(v);
    }
    //tarjan
    for (int v = 0; v < n; v++) {
        if (!dfn[v]) tarjan_dfs(v);
    }
    for (int v = 0; v < n; v++) {
        for (int w : graph[v]) {
            if (color[v] == color[w]) continue;
            c_graph[color[v]].insert(color[w]);
        }
    }
    int ans = 0;
    for (int cv = 0; cv < color_cnt; cv++) {
        if (!dp[cv]) dp[cv] = dfs(cv);
        if (dp[cv] > ans) ans = dp[cv];
    }
    printf("%d\n", ans);
    return 0;
}

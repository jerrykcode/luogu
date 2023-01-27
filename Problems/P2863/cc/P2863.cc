#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
using namespace std;

#define N 10004

vector<int> g[N];

int dfn[N], low[N];
stack<int> s;
bool in_stack[N];
int idx, ans_cnt;

static void dfs(int v) {
    dfn[v] = low[v] = ++idx;
    s.push(v);
    in_stack[v] = true;
	for (auto it = g[v].begin(); it != g[v].end(); it++) {
        int w = *it;
        if (!dfn[w]) {
            dfs(w);
            if (low[w] < low[v]) low[v] = low[w];
        }
        else if (in_stack[w] && dfn[w] < low[v]) low[v] = dfn[w];
    }
    if (dfn[v] == low[v]) {
        int cnt = 0;
        while (1) {
            int w = s.top();
            s.pop();
            in_stack[w] = false;
            cnt++;
            if (w == v) break;
        }
        if (cnt > 1) ans_cnt++;
    }
}

static int tarjan(int n) {
    idx = ans_cnt = 0;
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) dfs(i);
    return ans_cnt;
}

int main() {
    int n, m, a, b;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &a, &b);
        g[a].push_back(b);
    }
    printf("%d", tarjan(n));
    exit(0);
}

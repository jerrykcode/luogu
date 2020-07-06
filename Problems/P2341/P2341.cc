#include <cstdio>
#include <cstdlib>
#include <stack>
#include <vector>
using namespace std;

#define maxn 10000

vector<int> graph[maxn];
int dfn[maxn], low[maxn];
stack<int> s;
bool in_stack[maxn];
int color[maxn];
int num[maxn];
bool has_in[maxn];

int color_cnt;
int dfs_cnt;
void dfs(int v) {
    dfn[v] = low[v] = ++dfs_cnt;
    s.push(v);
    in_stack[v] = true;
    for (int w : graph[v]) {
        if (!dfn[w]) {
            dfs(w);
            if (low[w] < low[v]) low[v] = low[w];
        }
        else if (in_stack[w]) {
            if (dfn[w] < low[v]) low[v] = dfn[w];
        }        
    }
    if (dfn[v] == low[v]) {
        int w;
        while ((w = s.top()) != v) {
            color[w] = color_cnt;
            num[color_cnt]++;
            s.pop();
            in_stack[w] = false;
        }
        color[s.top()] = color_cnt;
        num[color_cnt]++;
        s.pop();
        in_stack[v] = false;
        color_cnt++;
    }
}

int main () {
    int n, m, a, b;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &a, &b);
        a--; b--;
        graph[b].push_back(a);
    }
    //tarjan
    for (int v = 0; v < n; v++)
        if (!dfn[v])
            dfs(v);
    for (int v = 0; v < n; v++)
        for (int w : graph[v]) {
            if (color[v] != color[w]) has_in[color[w]] = true;
        }
    int ans = 0;
    for (int i = 0; i < color_cnt; i++) {
        if (!has_in[i]) {
            if (ans) {
                printf("0\n");
                goto END;
            }
            ans = num[i];
        }
    }
    printf("%d\n", ans);
END:
    return 0;
}

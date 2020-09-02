#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;
#include <stack>
using std::stack;

#define maxn 100

vector<int> graph[maxn];
int n;

int dfn[maxn], low[maxn], count;
stack<int> s;
bool in_stack[maxn];
int color[maxn], color_count;
int in[maxn], out[maxn];

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

void Dfs(int v) {
    dfn[v] = low[v] = count++;
    s.push(v);
    in_stack[v] = true;
    for (auto it = graph[v].begin(); it != graph[v].end(); it++) {
        if (!dfn[*it]) {
            Dfs(*it);
            low[v] = min(low[v], low[*it]);
        }
        else if (in_stack[*it])
            low[v] = min(low[v], dfn[*it]);
    }
    if (dfn[v] == low[v]) {
        while (true) {
            int w = s.top();
            s.pop();
            in_stack[w] = false;
            color[w] = color_count;
            if (w == v) break;
        }
        color_count++;
    }
}

void Tarjan() {
    count = 1;
    color_count = 0;
    for (int v = 0; v < n; v++)
        if (!dfn[v]) 
            Dfs(v);
}

int main() {
    scanf("%d", &n);
    for (int v = 0; v < n; v++) {
        int w;
        while (true) {
            scanf("%d", &w);
            if (!w) break;
            graph[v].push_back(w - 1);
        }
    }
    Tarjan();
    if (color_count == 1) {
        printf("1\n0");
        exit(0);
    }
    for (int v = 0; v < n; v++)
        for (auto it = graph[v].begin(); it != graph[v].end(); it++) 
            if (color[v] != color[*it]) {
                in[color[*it]]++;
                out[color[v]]++;
            }
    int in0 = 0, out0 = 0;
    for (int c = 0; c < color_count; c++) {
        if (in[c] == 0) in0++;
        if (out[c] == 0) out0++;
    }
    printf("%d\n%d", in0, max(in0, out0));
    exit(0);
}

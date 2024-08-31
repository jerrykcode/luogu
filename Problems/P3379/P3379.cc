#include <cstdio>
#include <cstdlib>
#include <vector>
#define N 500005
std::vector<int> t[N];
int son[N], dep[N], fa[N], top[N];
inline int dfs1(int x) {
    int sz = 1, max_son_sz = 0, son_sz;
    for (auto it : t[x]) {
        if (it != fa[x]) {
            fa[it] = x;
            dep[it] = dep[x] + 1;
            sz += (son_sz = dfs1(it));
            if (son_sz > max_son_sz) {
                max_son_sz = son_sz;
                son[x] = it;
            }
        }
    }
    return sz;
}
void dfs2(int x, int tx) {
    top[x] = tx;
    if (son[x]) dfs2(son[x], tx);
    for (auto it : t[x]) {
        if (it != fa[x] && it != son[x]) {
            dfs2(it, it);
        }
    }
}
int main() {
    int n, m, s, x, y;
    scanf("%d %d %d", &n, &m, &s);
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d", &x, &y);
        t[x].push_back(y);
        t[y].push_back(x);
    }
    dfs1(s);
    dfs2(s, s);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &x, &y);
        int tx = top[x], ty = top[y];
        while (tx != ty) {
            if (dep[tx] < dep[ty]) {
                y = fa[ty];
                ty = top[y];
            }
            else {
                x = fa[tx];
                tx = top[x];    
            }
        }
        printf("%d\n", dep[x] < dep[y] ? x : y);
    }
}

#include <cstdio>
#include <cstdlib>
#include <vector>
#define N 100005
std::vector<int> tree[N];
int val[N], dep[N], fa[N], son[N], dfn[N], rnk[N], top[N], bottom[N];
int p, dfn_count;
struct { int l, r, sum, lazy; } sgt[N << 2 | 1];
#define sgt_left(i) (i << 1 | 1)
#define sgt_right(i) (sgt_left(i) + 1)
inline void sgt_build(int i, int l, int r) {
    if (l == r) { sgt[i] = {.l = l, .r = r, .sum = val[rnk[l]] % p, .lazy = 0 }; return; }
    int mid = (l + r) >> 1;
    sgt_build(sgt_left(i), l, mid);
    sgt_build(sgt_right(i), mid + 1, r);
    sgt[i] = { .l = l, .r = r, .sum = (sgt[sgt_left(i)].sum + sgt[sgt_right(i)].sum) % p, .lazy = 0 }; 
}
inline bool sgt_contain(int l, int r, int i) { return l <= sgt[i].l && sgt[i].r <= r; }
inline bool sgt_cross(int l, int r, int i) { return r >= sgt[i].l && l <= sgt[i].r; }
inline int sgt_len(int i) { return sgt[i].r - sgt[i].l + 1; }
inline void lazy_down(int i) {
    if (!sgt[i].lazy || sgt[i].l == sgt[i].r) return;
    sgt[sgt_left(i)].sum = (sgt[sgt_left(i)].sum + sgt_len(sgt_left(i)) * sgt[i].lazy) % p;
    sgt[sgt_right(i)].sum = (sgt[sgt_right(i)].sum + sgt_len(sgt_right(i)) * sgt[i].lazy) % p;
    sgt[sgt_left(i)].lazy += sgt[i].lazy;
    sgt[sgt_right(i)].lazy += sgt[i].lazy;
    sgt[i].lazy = 0;
}
inline void sgt_update(int i, int l, int r, int incr) {
    if (sgt_contain(l, r, i)) {
        sgt[i].sum = (sgt[i].sum + incr * sgt_len(i)) % p;
        sgt[i].lazy += incr;
        return;
    }
    lazy_down(i);
    if (sgt_cross(l, r, sgt_left(i))) sgt_update(sgt_left(i), l, r, incr);
    if (sgt_cross(l, r, sgt_right(i))) sgt_update(sgt_right(i), l, r, incr);
    sgt[i].sum = (sgt[sgt_left(i)].sum + sgt[sgt_right(i)].sum) % p;
}
inline int sgt_query(int i, int l, int r) {
    if (sgt_contain(l, r, i)) return sgt[i].sum % p;
    lazy_down(i);
    int res = 0;
    if (sgt_cross(l, r, sgt_left(i))) res = (res + sgt_query(sgt_left(i), l, r)) % p;
    if (sgt_cross(l, r, sgt_right(i))) res = (res + sgt_query(sgt_right(i), l, r)) % p;
    return res;
}
int dfs1(int i) {
    int sz = 1, son_sz, max_son_sz = 0;
    son[i] = -1;
    for (auto v : tree[i]) {
        if (v != fa[i]) {
            fa[v] = i;
            dep[v] = dep[i] + 1;
            if ((son_sz = dfs1(v)) > max_son_sz) { max_son_sz = son_sz; son[i] = v; }
            sz += son_sz;
        }
    }
    return sz;
}
void dfs2(int i, int t) {
    top[i] = t;
    dfn[i] = ++dfn_count;
    rnk[dfn[i]] = i;
    if (son[i] != -1) {
        dfs2(son[i], t);
        for (auto v : tree[i])
            if (v != fa[i] && v != son[i]) dfs2(v, v);
    }
    bottom[i] = dfn_count;
}
inline void update(int x, int y, int incr) {
    int tx = top[x], ty = top[y];
    while (tx != ty) {
        if (dep[tx] > dep[ty]) {
            sgt_update(0, dfn[tx], dfn[x], incr); x = fa[tx]; tx = top[x];
        } else {
            sgt_update(0, dfn[ty], dfn[y], incr); y = fa[ty]; ty = top[y];
        }
    }
    if (dep[x] > dep[y]) sgt_update(0, dfn[y], dfn[x], incr); else sgt_update(0, dfn[x], dfn[y], incr);
}
inline void update_root(int x, int incr) { sgt_update(0, dfn[x], bottom[x], incr); }
inline int query(int x, int y) {
    int tx = top[x], ty = top[y], res = 0;
    while (tx != ty) {
        if (dep[tx] > dep[ty]) {
            res = (res + sgt_query(0, dfn[tx], dfn[x])) % p; x = fa[tx]; tx = top[x];
        } else {
            res = (res + sgt_query(0, dfn[ty], dfn[y])) % p; y = fa[ty]; ty = top[y];
        }
    }
    if (dep[x] > dep[y]) return (res + sgt_query(0, dfn[y], dfn[x])) % p; else return (res + sgt_query(0, dfn[x], dfn[y])) % p;
}
inline int query_root(int x) { return (sgt_query(0, dfn[x], bottom[x])); }
int main() {
    int n, m, r, i, x, y, cmd, z;
    scanf("%d %d %d %d", &n, &m, &r, &p);
    for (i = 1; i <= n; i++) scanf("%d", &val[i]);
    for (i = 0; i < n - 1; i++) {
        scanf("%d %d", &x, &y);
        tree[x].push_back(y);
        tree[y].push_back(x);
    }
    fa[r] = -1, dep[r] = 0;
    dfs1(r);
    dfn_count = 0;
    dfs2(r, r);
    sgt_build(0, 1, dfn_count);
    for (i = 0; i < m; i++) {
        scanf("%d", &cmd);
        if (cmd == 1) {
            scanf("%d %d %d", &x, &y, &z);
            update(x, y, z);
        } else if (cmd == 2) {
            scanf("%d %d", &x, &y);
            printf("%d\n", query(x, y));
        } else if (cmd == 3) {
            scanf("%d %d", &x, &z);
            update_root(x, z);
        } else {
            scanf("%d", &x);
            printf("%d\n", query_root(x));
        }
    }
}

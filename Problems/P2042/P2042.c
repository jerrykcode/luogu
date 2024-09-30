#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define N 500005
typedef unsigned int ptr_t;
#define nullptr 0
struct {
    ptr_t fa;
    ptr_t ch[2];
    int val, sz;
} mem[N];
#define get(p, field) (mem[p].field)
#define set(p, field, val) if (p != nullptr) mem[p].field = val;
ptr_t last;
static inline void init() { last = nullptr; }
static inline ptr_t talloc() { return ++last; } // change to bitmap later
static inline ptr_t create(int val) {
    ptr_t p = talloc();
    set(p, val, val);
    set(p, sz, 1);
    return p;
}
static inline void update_sz(ptr_t p) {
    if (p != nullptr) {
        int sz = 1;
        if (get(p, ch[0]) != nullptr) sz += get(get(p, ch[0]), sz);
        if (get(p, ch[1]) != nullptr) sz += get(get(p, ch[1]), sz);
        set(p, sz, sz);
    }
}
int a[N];
static inline ptr_t build(int l, int r, ptr_t fa) {
    if (l > r) return nullptr;
    int mid = (l + r) >> 1;
    ptr_t p = create(a[mid]);
    set(p, fa, fa);
    set(p, ch[0], build(l, mid - 1, p));
    set(p, ch[1], build(mid + 1, r, p));
    update_sz(p);
    return p;
}
ptr_t rt;
static inline int which_ch(ptr_t p) {
    return get(get(p, fa), ch[1]) == p;
}
static inline void rotate(ptr_t p) {
    if (p == nullptr) return;
    ptr_t fa = get(p, fa);
    if (fa == nullptr) return;
    ptr_t g = get(fa, fa);
    int p_which_ch = which_ch(p);
    int fa_which_ch = which_ch(fa);
    ptr_t ch = get(p, ch[1 ^ p_which_ch]);
    set(fa, ch[p_which_ch], ch);
    set(ch, fa, fa);
    set(p, ch[1 ^ p_which_ch], fa);
    set(fa, fa, p);
    update_sz(fa);
    update_sz(p);
    set(p, fa, g);
    if (g == nullptr) rt = p;
    else set(g, ch[fa_which_ch], p);
}
static inline void splay(ptr_t p, ptr_t goal) {
    for (ptr_t fa, g; (fa = get(p, fa)) != goal; rotate(p))
        if ((g = get(fa, fa)) != goal) rotate(which_ch(p) ^ which_ch(fa) ? p : fa);
}
static inline ptr_t kth(int k) {
    ptr_t cur = rt;
    while (cur != nullptr) {
        int left_ch_sz = get(cur, ch[0]) == nullptr ? 0 : get(get(cur, ch[0]), sz);
        if (k < left_ch_sz + 1) {
            cur = get(cur, ch[0]);
        } else if (k == left_ch_sz + 1) {
            return cur;
        } else {
            k -= left_ch_sz + 1;
            cur = get(cur, ch[1]);
        }
    }
    return nullptr;
}
static inline void insert(int pos, int tot) {
    ptr_t p = kth(pos + 1);
    splay(p, 0);
    ptr_t pnext = kth(pos + 2);
    splay(pnext, p);
    for (int i = 0; i < tot; i++) scanf("%d", &a[i]);
    set(pnext, ch[0], build(0, tot - 1, pnext));
    update_sz(pnext);
    update_sz(p);
}
static inline void dfs(ptr_t p) {
    if (p == nullptr) return;
    dfs(get(p, ch[0]));
    printf("%d ", get(p, val));
    dfs(get(p, ch[1]));
}
int main() {
    init();
    int n, m;
    scanf("%d %d", &n, &m);
    a[0] = 520;
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    a[n + 1] = 1314;
    rt = build(0, n + 1, nullptr);
    char cmd[64];
    int pos, tot;
    while (m--) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "INSERT")) {
            scanf("%d %d", &pos, &tot);
            insert(pos, tot);
            dfs(rt);
        }
    }
    exit(0);
}

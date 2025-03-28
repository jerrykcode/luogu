#include "stdio.h"
#include "stdlib.h"
#define N 100005

struct {
    int val, id;
    int dist;
    int l, r;
} t[N];

int del[N];
int rt[N];

static inline int find(int x) { return rt[x] == x ? x : (rt[x] = find(rt[x])); }

static inline void swap(int *x, int *y) { 
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

static inline int merge(int x, int y) {
    if (x == 0 || y == 0) return x + y;
    if (t[y].val < t[x].val || (t[y].val == t[x].val && t[y].id < t[x].id)) swap(&x, &y);
    t[x].r = merge(t[x].r, y);
    if (t[t[x].r].dist > t[t[x].l].dist) {
        swap(&t[x].l, &t[x].r);
    }
    t[x].dist = t[t[x].r].dist + 1;
    return x;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &t[i].val);
        t[i].dist = 1;
        t[i].id = i;
        t[i].l = t[i].r = 0;
        rt[i] = i;
    }
    int cmd, x, y;
    while (m--) {
        scanf("%d %d", &cmd, &x);
        if (cmd == 1) {
            scanf("%d", &y);
            if (del[x] || del[y]) continue;
            x = find(x);
            y = find(y);
            if (x != y)
                rt[x] = rt[y] = merge(x, y);
        } else {
            if (del[x]) { printf("-1\n"); continue; }
            x = find(x);
            printf("%d\n", t[x].val);
            del[x] = 1;
            rt[t[x].l] = rt[t[x].r] = rt[x] = merge(t[x].l, t[x].r);
        }
    }
    exit(0);
}

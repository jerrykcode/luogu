#include "stdio.h"
#include "stdlib.h"

#define N 100005

struct {
    int dist;
    int l, r;
    int del;
    long long val;
} t[N];

int rt[N];

static inline int get_rt(int x) {
    if (rt[x] == x) return x;
    return rt[x] = get_rt(rt[x]);
}

static inline void swap(int *p0, int *p1) {
    *p0 += *p1;
    *p1 = *p0 - *p1;
    *p0 -= *p1;
}

static inline int merge(int x, int y) {
    if (!x || !y) return x + y;
    if (t[x].val > t[y].val || (t[x].val == t[y].val && x > y)) swap(&x, &y);
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
    t[0].dist = 0;
    t[0].l = t[0].r = 0;
    rt[0] = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &t[i].val);
        t[i].dist = 1;
        t[i].l = t[i].r = 0;
        t[i].del = 0;
        rt[i] = i;
    }
    int cmd, x, y;
    while (m--) {
        scanf("%d %d", &cmd, &x);
        if (cmd == 1) {
            scanf("%d", &y);
            if (t[x].del || t[y].del) continue;
            x = get_rt(x);
            y = get_rt(y);
            if (x != y)
                rt[x] = rt[y] = merge(x, y);
        } else {
            if (t[x].del) {
                printf("-1\n");
                continue;
            }
            x = get_rt(x);
            printf("%lld\n", t[x].val);
            t[x].del = 1;
            rt[x] = rt[t[x].l] = rt[t[x].r] = merge(t[x].l, t[x].r);
        }
    }
    exit(0);
}

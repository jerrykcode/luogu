#include <stdio.h>
#include <stdlib.h>

#define MAXN 5000

struct Stick {
    int l, w;
} st[MAXN];

int cmp(const void *a, const void *b) {
    struct Stick *st_a = (struct Stick *)a;
    struct Stick *st_b = (struct Stick *)b;
    return st_a->l != st_b->l ? st_b->l - st_a->l : st_b->w - st_a->w;
}

int dp[MAXN];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &st[i].l, &st[i].w);
    }    
    qsort(st, n, sizeof(struct Stick), cmp);
    int ans = 0;
    for (int i = 0; i < n; i++) {
        dp[i] = 1; //Init
        for (int j = 0; j < i; j++)
            if (st[j].w < st[i].w)
                dp[i] = max(dp[i], dp[j] + 1);
        ans = max(ans, dp[i]);
    }
    printf("%d\n", ans);
}

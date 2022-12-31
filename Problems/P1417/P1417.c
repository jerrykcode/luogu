#include <stdio.h>
#include <stdlib.h>

#define N 50
#define T 100005

typedef long long ll;

struct food {
    int a, b, c;
}f[N];

int compare(const void *x, const void *y) {
    return ((ll)((struct food *)x)->c * (ll)((struct food *)y)->b
             < (ll)((struct food *)y)->c * (ll)((struct food *)x)->b) ? -1 : 1;
}

ll dp[T];

static inline ll max(ll a, ll b) { return a > b ? a : b; }

int main() {
    int t, n, i, j;
    scanf("%d %d", &t, &n);
    for (i = 0; i < n; i++) scanf("%d", &f[i].a);
    for (i = 0; i < n; i++) scanf("%d", &f[i].b);
    for (i = 0; i < n; i++) scanf("%d", &f[i].c);
    qsort(f, n, sizeof(f[0]), compare);
    for (i = 0; i < n; i++)
        for (j = t; j >= f[i].c; j--)
            dp[j] = max(dp[j], dp[j - f[i].c] + f[i].a - j * (ll)f[i].b);
    ll ans = 0ll;
    for (i = 0; i <= t; i++)
        ans = max(ans, dp[i]);
    printf("%lld", ans);
    exit(0);
}

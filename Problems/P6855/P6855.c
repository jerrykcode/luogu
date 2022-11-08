#include <stdio.h>
#include <stdlib.h>

#define N 2003

typedef long long ll;

int a[N][N];
ll f[N][N], g[N][N], l[N][N], d[N][N];

static ll max(ll a, ll b) { return a > b ? a : b; }
static ll min(ll a, ll b) { return a < b ? a : b; }
static ll max3(ll a, ll b, ll c) { return max(a, max(b, c)); }

int main() {
    int n, m, r, c;
    ll ans;
    scanf("%d %d", &n, &m);
    for (r = 1; r <= n; r++)
        for (c = 1; c <= m; c++) {
            scanf("%d", &a[r][c]);
            f[r][c] = max(f[r][c-1], f[r-1][c]) + a[r][c];
        }
    for (r = n; r; r--)
        for (c = m; c; c--)
            g[r][c] = max(g[r][c+1], g[r+1][c]) + a[r][c];
    ans = 0x7fffffffffffffff;
    for (r = 1; r <= n; r++)
        for (c = 1; c <= m; c++) {
            l[r][c] = max(f[r][c-1] + g[r+1][c-1], l[r][c-1]);
            d[r][c] = max(f[r-1][c] + g[r-1][c+1], d[r-1][c]);
            ans = min(ans, max3(l[r][c], d[r][c], f[r][c] + g[r][c] - 2*a[r][c]));
        }
    printf("%lld", ans);
    exit(0);
}

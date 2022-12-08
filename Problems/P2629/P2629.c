#include <stdio.h>
#include <stdlib.h>
#define N 1000006
int a[N], sum[N], f[N], g[N];
static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, i, ans = 0;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        sum[i] = a[i] + (i ? sum[i - 1] : 0);
    }
    f[n - 1] = a[n - 1];
    for (i = n - 2; i >= 0; i--)
        f[i] = min(a[i], a[i] + f[i + 1]);
    g[0] = a[0];
    for (i = 1; i < n; i++)
        g[i] = min(g[i - 1], sum[i]);
    for (i = 0; i < n; i++)
        if (f[i] >= 0 && ( i == 0 || sum[n - 1] - sum[i - 1] + g[i - 1] >= 0)) ans++;
    printf("%d", ans);
    exit(0);
}

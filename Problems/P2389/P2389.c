#include <stdio.h>
#include <stdlib.h>

#define N 505

int sum[N];
int f[N], g[N];

static inline int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, k, i;
    scanf("%d %d", &n, &k);
    for (i = 1; i <= n; i++) {
        scanf("%d", &sum[i]);
        sum[i] += sum[i - 1];
    }
    int seg, tmp, ans = 0;
    for (seg = 1; seg <= k; seg++) {
        tmp = 0;
        for (i = seg; i <= n; i++) {
            f[i] = tmp + sum[i];
            tmp = max(tmp, g[i] - sum[i]);
            g[i] = max(g[i - 1], f[i]);
        }
        ans = max(ans, g[n]);
    }
    printf("%d\n", ans);
    exit(0);
}

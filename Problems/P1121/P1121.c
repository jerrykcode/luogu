#include <stdio.h>
#include <stdlib.h>

#define N 200005
#define MIN 0x80000000
#define MAX 0x7fffffff

int sum[N];
int f[N];
int g[N];

static inline int min(int a, int b) { return a < b ? a : b; }
static inline int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, i;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &sum[i]);
        if (i) sum[i] += sum[i - 1];
    }
    int tmp = 0;
    for (i = 0; i < n; i++) {
        f[i] = sum[i] - tmp;
        tmp = min(tmp, sum[i]);
        g[i] = (i ? max(g[i - 1], f[i]) : f[i]);
    }
    int ans = MIN;
    tmp = 0;  // g[0] - sum[0];
    for (i = 1; i < n; i++) {
        f[i] = sum[i] + tmp;
        tmp = max(tmp, g[i] - sum[i]);
        g[i] = ((i > 1) ? max(g[i - 1], f[i]) : f[i]);
        ans = max(ans, g[i]);
    }
    tmp = 0;
    for (i = 1; i < n - 1; i++)
        sum[i] -= sum[0];
    for (i = 1; i < n - 1; i++) {
        f[i] = sum[i] - tmp;
        tmp = max(tmp, sum[i]);
        g[i] = ((i > 1) ? min(g[i - 1], f[i]) : f[i]);
    }
    int minAns = MAX;
    tmp = 0;  // g[0] - sum[0]
    for (i = 2; i < n - 1; i++) {
        f[i] = sum[i] + tmp;
        tmp = min(tmp, g[i] - sum[i]);
        g[i] = ((i > 2) ? min(g[i - 1], f[i]) : f[i]);
        minAns = min(minAns, g[i]);
    }
    ans = max(ans, sum[n - 1] - minAns);
    printf("%d", ans);
    exit(0);
}

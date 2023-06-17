#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100

int a[N], f[N], g[N];

static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, i, j, ans;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    memset(f, 0x3f, sizeof(f));
    f[0] = 0;
    for (i = 1; i < n; i++) {
        f[i] = i;
        for (j = i - 1; j >= 0; j--) {
            if (a[j] < a[i]) {
                f[i] = min(f[i], f[j] + i - j - 1);
            }
        }
    }
    memset(g, 0x3f, sizeof(g));
    g[n - 1] = 0;
    for (i = n - 2; i >= 0; i--) {
        g[i] = n - 1 - i;
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[i]) {
                g[i] = min(g[i], g[j] + j - i - 1);
            }
        }
    }
    ans = 0x3f3f3f3f;
    for (i = 0; i < n; i++) {
        ans = min(ans, f[i] + g[i]);
    }
    printf("%d", ans);
    exit(0);
}

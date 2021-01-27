#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define maxn 1000
int pre[maxn];
int dp[maxn][maxn];
int f[maxn][maxn], g[maxn][maxn];

int sum(int l, int r) {
    return pre[r] - (l ? pre[l - 1] : 0);
}

int min(const int argc, ...) {
    va_list args;
    va_start(args, argc);
    int res = va_arg(args, int), tmp;
    for (int i = 1; i < argc; i++)
        if ((tmp = va_arg(args, int)) < res) res = tmp;
    return res;
}

int main() {
    int t, n, i, len, l, r;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (i = 0; i < n; i++) {
            scanf("%d", &pre[i]);
            dp[i][i] = f[i][i] = g[i][i] = pre[i];
            if (i) pre[i] += pre[i - 1];
        }
        for (len = 2; len <= n; len++) 
            for (l = 0; l < n - len + 1; l++) {
                r = l + len - 1;
                dp[l][r] = sum(l, r) - min(3, f[l + 1][r], g[l][r - 1], 0);
                f[l][r] = min(2, f[l + 1][r], dp[l][r]);
                g[l][r] = min(2, g[l][r - 1], dp[l][r]);
            }
        printf("%d\n", dp[0][n - 1]);
    }
    exit(0);
}

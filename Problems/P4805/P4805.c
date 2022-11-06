#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 400

int a[N];
int presum[N];
int8_t f[N][N];

int main() {
    int n, i, j, len, start, end, l, r, ans = 0, tmp;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        presum[i] = a[i] + (i ? presum[i - 1] : 0);
        f[i][i] = 1;
        if (a[i] > ans) ans = a[i];
    }
    for (len = 2; len <= n; len++)
        for (start = 0, end = len - 1; end < n; start++, end++) {
            i = start, j = end;
            l = a[i], r = a[j];
            while (i < j) {
                if (l < r) { l += a[++i]; continue; }
                if (l > r) { r += a[--j]; continue; }
                // l == r
                if (f[start][i] && f[j][end] && (i + 1 > j - 1 || f[i + 1][j - 1])) { 
                    f[start][end] = 1;
                    tmp = presum[end] - (start ? presum[start - 1] : 0);
                    if (tmp > ans) ans = tmp;
                    break;
                } else {
                    l += a[++i];
                    r += a[--j];
                }
            }
        }
    printf("%d", ans);
    exit(0);
}

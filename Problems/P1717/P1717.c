#include "stdio.h"
#include "stdlib.h"

#define MAXH 16 * 12
#define MAXN 25

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef long long rt;  // result type

rt dp[MAXN][MAXH + 1];
int f[MAXN], d[MAXN], t[MAXN];

int main() {
    int n, h;
    scanf("%d %d", &n, &h);
    h *= 12;
    int i, j, k, tmp_f, fs, sum_t;
    for (i = 0; i < n; i++) scanf("%d", &f[i]);
    for (i = 0; i < n; i++) scanf("%d", &d[i]);
    for (i = 0; i < n - 1; i++) scanf("%d", &t[i]);
    sum_t = 0;
    rt result;
    fs = 0;
    for (i = 0; i <= h; i++) {
        dp[0][i] = fs;
        fs += f[0];
        f[0] = max(0, f[0] - d[0]);
    }
    result = dp[0][h];
    sum_t += t[0];
    for (i = 1; i < n; i++) {
        for (j = sum_t; j <= h; j++) {
            fs = 0;
            tmp_f = f[i];
            for (k = 0; k <= j - sum_t; k++) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - k - t[i - 1]] + fs);
                if (tmp_f <= 0) break;
                fs += tmp_f;
                tmp_f -= d[i];
            }
        }
        sum_t += t[i - 1];
        result = max(result, dp[i][h]);
    }
    printf("%lld\n", result);
    exit(0);
}

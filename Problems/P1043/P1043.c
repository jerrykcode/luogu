#include "stdio.h"
#include "stdlib.h"

#define MAXN 50
#define MAXM 9

typedef long long rt;
#define INF 0x7fffffffffffffff

int arr[MAXN << 1];
int sum[MAXN << 1];
rt f[MAXN << 1][MAXN << 1][MAXM + 1];
rt g[MAXN << 1][MAXN << 1][MAXM + 1];

#define FLOOR10(x) ((x) > 0 ? (x)/10 : ((x) % 10 ? (x)/10 - 1 : (x)/10))
#define MOD10(x) ((x) - FLOOR10(x) * 10)
#define SUM(i, j) ((i) > 0 ? sum[j] - sum[i - 1] : sum[j])
#define MOD(i, j) MOD10(SUM(i, j))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int i, j, k, l;
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        sum[i] = arr[i];
        if (i) sum[i] += sum[i - 1];
    }
    for (; i < n << 1; i++) {
        arr[i] = arr[i - n];
        sum[i] = arr[i] + sum[i - 1];
    }
    for (i = 0; i < n << 1; i++)
        for (j = i; j < n << 1; j++) {
            f[i][j][1] = g[i][j][1] = MOD(i, j);
            for (k = 2; k <= m; k++)
                g[i][j][k] = INF;
        }

    for (int len = 2; len <= n; len++) {
        for (i = 0; i <= (n << 1) - len; i++) {
            j = i + len - 1;
            for (k = 2; k <= MIN(len, m); k++) {
                for (l = i + k - 1; l <= j; l++) {
                    f[i][j][k] = MAX(f[i][j][k], f[i][l - 1][k - 1] * MOD(l, j));
                    g[i][j][k] = MIN(g[i][j][k], g[i][l - 1][k - 1] * MOD(l, j));
                }
            }
        }
    }

    rt ans_max = 0, ans_min = INF;
    for (i = 0; i <= n; i++) {
        ans_max = MAX(ans_max, f[i][i + n - 1][m]);
        ans_min = MIN(ans_min, g[i][i + n - 1][m]);
    }
    printf("%lld\n%lld\n", ans_min, ans_max);
    exit(0);
}

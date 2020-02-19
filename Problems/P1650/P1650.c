#include "stdio.h"
#include "stdlib.h"

#define maxn 2001
int a[maxn], b[maxn];
int dp[maxn][maxn];

int max(int x, int y) { 
    return x > y ? x : y; 
}

int race(int x, int y) { 
    int d = a[x] - b[y];
    if (d > 0) return 200;
    else if (d < 0) return -200;
    else return 0;
}

int compare(const void * x, const void * y) {
    return *((int *)y) - *((int *)x);
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", a + i);
    for (int i = 1; i <= n; i++) scanf("%d", b + i);
    qsort(a + 1, n, sizeof(int), compare);
    qsort(b + 1, n, sizeof(int), compare);
    for (int i = 1; i <= n; i++) {
        dp[i][0] = dp[i - 1][0] + race(n - i + 1, i);
        dp[i][i] = dp[i - 1][i - 1] + race(i, i);
        for (int j = 1; j < i; j++) {
            dp[i][j] = max(dp[i - 1][j - 1] + race(j, i),
                           dp[i - 1][j] + race(n - i + j + 1, i));
        }
    }
    int res = dp[n][1];
    for (int i = 2; i <= n; i++)
        res = max(res, dp[n][i]);
    printf("%d", res);
    return 0;
}

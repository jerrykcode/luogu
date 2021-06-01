#include <stdio.h>
#include <stdlib.h>

#define MAXN 30000

int dp[MAXN + 1];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int v, p;
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &v, &p);
        p *= v;
        for (int j = n; j >= v; j--)
            dp[j] = max(dp[j], dp[j - v] + p);
    }
    printf("%d\n", dp[n]);
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>

#define MAXT 1000
int dp[MAXT + 1];

int max(int a , int b) { return a > b ? a : b; }

int main() {
    int t, m, ti, vi;
    scanf("%d %d", &t, &m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &ti, &vi);
        for (int j = t; j >= ti; j--)
            dp[j] = max(dp[j], dp[j - ti] + vi);
    }
    printf("%d\n", dp[t]);
    exit(0);
}

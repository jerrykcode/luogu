#include "stdio.h"
#include "stdlib.h"

int dp[20001];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int v, n, vi;
    scanf("%d", &v);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &vi);
        for (int j = v; j >= vi; j--)
            dp[j] = max(dp[j], dp[j - vi] + vi);
    }
    printf("%d", v - dp[v]);
    return 0;
}

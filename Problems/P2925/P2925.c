#include "stdio.h"
#include "stdlib.h"

int dp[50001];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int c, h, v;
    scanf("%d %d", &c, &h);
    for (int i = 0; i < h; i++) {
        scanf("%d", &v);
        for (int j = c; j >= v; j--) {
            dp[j] = max(dp[j], dp[j - v] + v);
            if (i == h - 1) break;
        }
    }
    printf("%d", dp[c]);
    return 0;
}

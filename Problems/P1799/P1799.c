#include "stdio.h"
#include "stdlib.h"

int dp[1001];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n, a, res = 0;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a);
        for (int j = i; j >= 1; j--) {
            dp[j] = max(dp[j], dp[j - 1] + (a == j ? 1 : 0));
            if (dp[j] > res) res = dp[j];
        }
    }
    printf("%d", res);
    return 0;
}

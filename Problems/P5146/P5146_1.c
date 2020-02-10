#include "stdio.h"
#include "stdlib.h"

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
int dp[1000000];

int main() {
    int n, res = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", dp + i);
        if (i) {
            res = max(res, dp[i] - dp[i - 1]);
            dp[i] = min(dp[i], dp[i - 1]);
        }
    }
    printf("%d", res);
    return 0;
}

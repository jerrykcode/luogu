#include "stdio.h"
#include "stdlib.h"

int dp[1000000];
int a[1000000];

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", a + i);
        dp[i] = i ? min(dp[i - 1], a[i]) : a[i];
    }
    int res = 0;
    for (int i = n - 1; i > 0; i--) {
        dp[i] = (i == n - 1) ? a[i] : max(a[i], dp[i + 1]);
        res = max(res, dp[i] - dp[i - 1]);
    }
    printf("%d", res);
    return 0;
}

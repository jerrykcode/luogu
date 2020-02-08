#include "stdio.h"
#include "stdlib.h"

int dp[31];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int t, w, a;
    scanf("%d %d", &t, &w);
    for (int i = 1; i <= t; i++) {
        scanf("%d", &a);
        for (int j = w; j >= 0; j--) {            
            if (j > 0) dp[j] = max(dp[j], dp[j - 1]);
            if (a == (j % 2 + 1)) dp[j]++;
        }
    }
    int res = 0;
    for (int j = 0; j <= w; j++)
        res = max(res, dp[j]);
    printf("%d", res);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_n 50
#define max_h 500000
#define max_h_sum max_h

int dp[max_n][max_h_sum];
int h_sum;

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n, h;
    h_sum = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        if (i) memcpy(dp[i], dp[i - 1], (h_sum + 1) * sizeof(int));
        scanf("%d", &h);
        h_sum += h;
        dp[i][h] = max(dp[i][h], h);
        if (!i) continue;
        for (int j = 0; j <= h_sum; j++) {
            if (dp[i - 1][j + h]) dp[i][j] = max(dp[i][j], dp[i - 1][j + h]);
            if (j > h) {
                if (dp[i - 1][j - h]) dp[i][j] = max(dp[i][j], dp[i - 1][j - h] + h);
            } 
            else {
                if (dp[i - 1][h - j]) dp[i][j] = max(dp[i][j], dp[i - 1][h - j] + j);
            }
        }
    }
    if (dp[n - 1][0]) 
        printf("%d\n", dp[n - 1][0]);
    else 
        printf("-1\n");
    exit(0);
}

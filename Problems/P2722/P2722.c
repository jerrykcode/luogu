#include "stdio.h"
#include "stdlib.h"

int dp[10001];
short score[10000];
short time[10000];

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    for (int i = 0; i < n; i++)
        scanf("%hd %hd", score + i, time + i);
    for (int i = 0; i < n; i++)
        for (int j = time[i]; j <= m; j++)
            dp[j] = max(dp[j], dp[j - time[i]] + score[i]);
    printf("%d", dp[m]);
    return 0;
}

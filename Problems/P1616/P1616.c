#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int main() {
    int t, m;
    scanf("%d %d", &t, &m);
    int * dp = (int *)malloc((t + 1) * sizeof(int));
    memset(dp, 0, (t + 1) * sizeof(int));
    short * time = (short *)malloc(m * sizeof(short));
    short * value = (short *)malloc(m * sizeof(short));
    for (int i = 0; i < m; i++) {
        scanf("%hd %hd", time + i, value + i);
    }
    for (int i = 0; i < m; i++)
        for (int j = time[i]; j <= t; j++)
            dp[j] = max(dp[j], dp[j - time[i]] + value[i]);
    printf("%d", dp[t]);
    free(dp);
    free(time);
    free(value);
    return 0;
}

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef unsigned char byte;

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int main () {
    int t, m;
    scanf("%d %d", &t, &m);
    short * dp = (short *)malloc((t + 1) * sizeof(short));
    memset(dp, 0, (t + 1) * sizeof(short));
    byte * time = (byte *)malloc(m * sizeof(byte));
    byte * value = (byte *)malloc(m * sizeof(byte));
    for (int i = 0; i < m; i++)
        scanf("%hhd %hhd", time + i, value + i);
    for (int i = 0; i < m; i++)
        for (int j = t; j >= time[i]; j--) {
            dp[j] = max(dp[j], dp[j - time[i]] + value[i]);
            if (i == m - 1) break;
        }
    printf("%hd", dp[t]);
    free(dp);
    free(time);
    free(value);
    return 0;
}

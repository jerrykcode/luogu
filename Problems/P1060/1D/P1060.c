#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef unsigned short word;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int * dp = (int *)malloc((n + 1) * sizeof(int));
    memset(dp, 0, (n + 1) * sizeof(int));
    word * cost = (word *)malloc(m * sizeof(word));
    word * value = (word *)malloc(m * sizeof(word));
    for (int i = 0; i < m; i++) {
        scanf("%hd %hd", cost + i, value + i);
        value[i] *= cost[i];
    }
    //背包
    for (int i = 0; i < m; i++)
        for (int j = n; j >= cost[i]; j--) {
            dp[j] = max(dp[j], dp[j - cost[i]] + value[i]);
            if (i == m - 1) break;
        }
    printf("%d", dp[n]);
    free(dp);
    free(cost);
    free(value);
    return 0;
}

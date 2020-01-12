#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef unsigned char byte;
typedef unsigned short word;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    word * dp = (word *)malloc((m + 1) * sizeof(word));
    memset(dp, 0, (m + 1) * sizeof(word));
    word * weight = (word *)malloc(n * sizeof(word));
    byte * value = (byte *)malloc(n * sizeof(byte));
    for (int i = 0; i < n; i++)
        scanf("%hd %hhd", weight + i, value + i);
    for (int i = 0; i < n; i++)
        for (int j = m; j >= weight[i]; j--) {
            dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
            if (i == n - 1) break;
        }
    printf("%hd", dp[m]);
    free(dp);
    free(weight);
    free(value);
    return 0;
}

#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef unsigned char byte;
typedef unsigned short word;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    word * weight = (word *)malloc((n + 1) * sizeof(word));
    byte * value = (byte *)malloc((n + 1) * sizeof(byte));
    for (int i = 1; i <= n; i++) {
        scanf("%hu %hhu", weight + i, value + i);
    }
    word **dp = (word **)malloc((m + 1) * sizeof(word *));
    for (int i = 0; i < m + 1; i++)
        dp[i] = (word *)malloc((n + 1) * sizeof(word));
    for (int i = 0; i < n + 1; i++) dp[0][i] = 0;
    for (int i = 0; i < m + 1; i++) dp[i][0] = 0;
    for (int i = 1; i < m + 1; i++)
        for (int j = 1; j < n + 1; j++) {
            if (i < weight[j])
                dp[i][j] = dp[i][j - 1];
            else
                dp[i][j] = max(dp[i][j - 1], dp[i - weight[j]][j - 1] + value[j]);
        }
    printf("%d", dp[m][n]);
    free(weight);
    free(value);
    for (int i = 0; i < m + 1; i++)
        free(dp[i]);
    free(dp);
    return 0;
}

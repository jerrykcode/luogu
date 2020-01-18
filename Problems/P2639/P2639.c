#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int main() {
    int h, n;
    scanf("%d %d", &h, &n);
    int * dp = (int *)malloc((h + 1) * sizeof(int));
    int * v = (int *)malloc(n * sizeof(int)); //价值与体积相同，一个数组就可以啦
    for (int i = 0; i < n; i++) scanf("%d", v + i);
    for (int i = 0; i < n; i++)
        for (int j = h; j >= v[i]; j--) {
            dp[j] = max(dp[j], dp[j - v[i]] + v[i]);
            if (i == n - 1) break;
        }
    printf("%d", dp[h]);
    free(dp);
    free(v);
    return 0;
}

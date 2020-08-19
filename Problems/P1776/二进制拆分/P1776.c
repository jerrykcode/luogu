#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define maxnum 2000
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int value[maxnum];
int weight[maxnum];

int main() {
    int n, w;
    scanf("%d %d", &n, &w);
    int * dp = (int *)malloc((w + 1) * sizeof(int));
    memset(dp, 0, (w + 1) * sizeof(int));
    int top = 0;
    int vi, wi;
    int num;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &vi, &wi, &num);
        for (int j = 1; j <= num; j <<= 1, top++) {
            num -= j;
            value[top] = j * vi;
            weight[top] = j * wi;
        }
        if (num > 0) {
            value[top] = num * vi;
            weight[top] = num * wi;
            top++;
        }
    }
    for (int i = 0; i < top; i++)
        for (int j = w; j >= weight[i]; j--) {
            dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
            if (i == top - 1) break;
        }
    printf("%d", dp[w]);
    free(dp);
    return 0;
}

#include "stdio.h"
#include "stdlib.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

int dp[1001];
int value[1001];
//weight[i] = i

int main() {
    int s;
    scanf("%d", &s);
    for (int i = 1; i <= s / 2; i++)
        for (int j = i + 1; j <= s; j++)
            if (j % i == 0) value[j] += i;
    for (int i = 0; i < s; i++)
        for (int j = s; j >= i; j--) {
            dp[j] = max(dp[j], dp[j - i] + value[i]);
            if (i == s - 1) break;
        }
    printf("%d", dp[s]);
    return 0;
}

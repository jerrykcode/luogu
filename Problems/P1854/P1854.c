#include "stdio.h"
#include "stdlib.h"

#define maxf 100
#define maxv 100

int dp[maxf + 1][maxv + 1];
int a[maxf + 1][maxv + 1];
int end[maxf + 1][maxv + 1];
int stack[maxf], top = 0;

const int min_inf = 0x80000000;

int main() {
    int f, v;
    scanf("%d %d", &f, &v);
    for (int i = 0; i <= f; i++)
        for (int j = 0; j <= v; j++)
            dp[i][j] = min_inf;
    for (int i = 1; i <= f; i++)
        for (int j = 1; j <= v; j++) {
            scanf("%d", &a[i][j]);
        }
    for (int j = 1; j <= v - f + 1; j++) {
        if (a[1][j] > dp[1][j - 1]) {
            dp[1][j] = a[1][j];
            end[1][j] = j;
        }
        else {
            dp[1][j] = dp[1][j - 1];
            end[1][j] = end[1][j - 1];
        }
    }
    for (int i = 2; i <= f; i++)
        for (int j = i; j <= v - f + i; j++) {
            if (dp[i - 1][j - 1] + a[i][j] > dp[i][j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + a[i][j];
                end[i][j] = j;
            }
            else {
                dp[i][j] = dp[i][j - 1];
                end[i][j] = end[i][j - 1];
            }
        }
    printf("%d\n", dp[f][v]);
    while (f) {
        stack[top++] = end[f][v];
        v = end[f][v] - 1;
        f--;
    }
    while (top) {
        printf("%d ", stack[--top]);        
    }
    return 0;
}

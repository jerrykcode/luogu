#include "stdio.h"
#include "stdlib.h"

#define maxv 400
#define maxw 400

int dp[maxv][maxw];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int v, w, n;
    scanf("%d %d %d", &v, &w, &n);
    int vi, wi, ci;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &vi, &wi, &ci);
        for (int j = v; j >= vi; j--)
            for (int k = w; k >= wi; k--)
                dp[j][k] = max(dp[j][k], dp[j - vi][k - wi] + ci);
    }
    printf("%d", dp[v][w]);
    return 0;
}

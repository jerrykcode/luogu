#include <stdio.h>
#include <stdlib.h>

#define MAXN 3000

int dp[MAXN + 1][MAXN + 1];

struct prod_st {
    int w;
    int r;
};

int compare(const void *a, const void *b) {
    return ((struct prod_st *)b)->r - ((struct prod_st *)a)->r;
}

struct prod_st products[MAXN];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &products[i].w, &products[i].r);
    }
    qsort(products, n, sizeof(struct prod_st), compare);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= i; j++)
            dp[i][j] = max(dp[i - 1][j], 
                dp[i - 1][j - 1] 
                + products[i - 1].w - (j - 1) * products[i - 1].r);
    int ans = 0;
    for (int i = 0; i <= n; i++)
        ans = max(ans, dp[n][i]);
    printf("%d\n", ans);
    exit(0);
}

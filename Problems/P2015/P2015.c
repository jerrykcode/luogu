#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100
#define Q 100

short mat[N][N];
int dp[N][Q+1];

static void init() {
    memset(mat, 0xff, sizeof(mat)); // -1
}

static int max(int a, int b) { return a > b ? a : b; }

static void DP(int v, int parent, int n, int q) {
    int left = -1, right;
    for (int i = 0; i < n; i++) //find left and right of v
        if (mat[v][i] != -1 && i != parent)
            if (left == -1) left = i; else right = i;
    if (left == -1)
        return;
    DP(left, v, n, q);
    DP(right, v, n, q);
    for (int i = 1; i <= q; i++) {
        dp[v][i] = dp[left][i - 1] + mat[v][left];
        dp[v][i] = max(dp[v][i], dp[right][i - 1] + mat[v][right]);
        for (int j = 0, k = i - 2; j <= i - 2; j++, k--)
            dp[v][i] = max(dp[v][i], dp[left][j] + dp[right][k] + mat[v][left] + mat[v][right]);
    }
}

int main() {
    init();
    int n, q;
    scanf("%d %d", &n, &q);
    for (int i = 0, a, b, c; i < n - 1; i++) {
        scanf("%d %d %d", &a, &b, &c);
        a--, b--;
        mat[a][b] = mat[b][a] = c;
    }
    DP(0, -1, n, q);
    printf("%d", dp[0][q]);
    exit(0);
}

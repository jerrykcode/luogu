#include "stdio.h"
#include "stdlib.h"

#define maxn 200
#define INF 0x7fffffff

int dp_max[maxn][maxn];
int dp_min[maxn][maxn];
int cost[maxn][maxn];

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int main() {
    int n;
    scanf("%d", &n);
    scanf("%d", &cost[0][0]);
    for (int i = 1; i < n; i++) {
        scanf("%d", &cost[i][i]);
        for (int j = 0; j < i; j++) {
            cost[j][i] = cost[j][i - 1]+ cost[i][i];
        }
    }
    for (int i = n; i < (n << 1); i++) {
        cost[i][i] = cost[i - n][i - n];
        for (int j = i - n + 1; j < i; j++) {
            cost[j][i] = cost[j][i - 1] + cost[i][i];
        }
    }
    for (int i = 0; i < (n << 1); i++) {
        for (int j = i + 1; j < (n << 1) && j < i + n; j++)
            dp_min[i][j] = INF;
    }
    for (int len = 2; len <= n; len++) {
        for (int l = 0, r = l + len - 1; r < (n << 1); l++, r++) {
            for (int k = l; k < r; k++) {
                dp_max[l][r] = max(dp_max[l][r], dp_max[l][k] + dp_max[k + 1][r] + cost[l][r]);
                dp_min[l][r] = min(dp_min[l][r], dp_min[l][k] + dp_min[k + 1][r] + cost[l][r]);
            }
        }
    }
    int ans_max = 0, ans_min = INF;
    for (int l = 0, r = l + n - 1; r < (n << 1); l++, r++) {
        ans_max = max(ans_max, dp_max[l][r]);
        ans_min = min(ans_min, dp_min[l][r]);
    }
    printf("%d\n%d", ans_min, ans_max);
    return 0;
}

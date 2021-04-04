#include <stdio.h>
#include <stdlib.h>

#define maxs 5001

int dp[2][maxs][2];

#define INF 0x7ffffffe

#define f(i, j, k) ((j >= 0 && j < maxs) ? dp[i][j][k] : INF)

int min2(int a, int b) { return a < b ? a : b; }

int min4(int a, int b, int c, int d) {
    return min2(min2(a, b), min2(c, d));
}

int main() {
    for (int i = 0; i < 2 ;i++) 
        for (int j = 0; j < maxs; j++) 
            for(int k = 0; k < 2; k++) dp[i][j][k] = INF;
    int cur = 0, pre = 1;
    dp[pre][0][0] = dp[pre][0][1] = 0;
    int n, a, b, s;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &a, &b);
        for (int j = 0; j <= i * 5; j++) {
            s = a - b;
            dp[cur][j][0] = min4(f(pre, j - s, 0), f(pre, j + s, 0) + 1,
                                f(pre, s - j, 1), f(pre, - s - j, 1) + 1);
            s = b - a;
            dp[cur][j][1] = min4(f(pre, j - s, 1), f(pre, j + s, 1) + 1,
                                f(pre, s - j, 0), f(pre, - s - j, 0) + 1);
        }
        cur = cur + pre;
        pre = cur - pre;
        cur = cur - pre;
    }
    int ans;
    for (int i = 0; i <= n * 5; i++)
        if ((ans = min2(dp[pre][i][0], dp[pre][i][1])) < INF) {
            printf("%d\n", ans);
            break;
        }
    exit(0);
}

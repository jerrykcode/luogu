#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define maxn 15
#define INF (0x7fffffff*1.0)

double dp[1<<maxn][maxn];
double x[maxn], y[maxn];

double square(double a) {
    return a*a;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt(square(x1 - x2) + square(y1 - y2));
}

double dist_index(int i, int j) {
    return dist(x[i], y[i], x[j], y[j]);
}

double min(double a, double b) {
    return a < b ? a : b;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &x[i], &y[i]);
        dp[1 << i][i] = dist(x[i], y[i], 0, 0); //Init
    }
    int state, pre, count;
    for (state = 3; state < 1 << n; state++) {
        count = 0;
        for (int i = 0; i < n; i++)
            if (state & (1 << i)) 
                count++;
        if (count <= 1) continue; //二进制位只有一个1的 在初始化时已经计算过
        for (int i = 0; i < n; i++) {
            if (state & (1 << i)) {
                dp[state][i] = INF; //Init
                pre = state ^ (1 << i); //state可以从pre转来
                for (int j = 0; j < n; j++)
                    if (pre & (1 << j)) {
                        dp[state][i] = min(
                            dp[state][i],
                            dp[pre][j] + dist_index(i, j)
                        );
                    }
            }
        }
    }
    double ans = INF;
    for (int i = 0; i < n; i++) {
        ans = min(ans, dp[(1 << n) - 1][i]);
    }
    printf("%0.2lf\n", ans);
    exit(0);
}

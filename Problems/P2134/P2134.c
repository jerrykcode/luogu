/*
dp[i][0] = min{dp[i - 1][0], dp[i - 1][1]} + q
dp[i][1] = min{dp[j][0] + (i - j)*(i - j)*p}, 0 <= j < i

斜率优化dp[i][1]:
对于dp[i][1], 求最优的j, 假设 0 <= j < k < i, 且 k优于j, 则:
    dp[k][0] + (i - k)*(i - k)*p < dp[j][0] + (i - j)*(i - j)*p,
令f[x] = dp[x][0], 即:
    f[k] + (i - k)*(i - k)*p < f[j] + (i - j)*(i - j)*p
=>  f[k] + i*i*p - 2*i*k*p + k*k*p < f[j] + i*i*p - 2*i*j*p + j*j*p
=>  f[k] - 2*i*p*k + k*k*p < f[j] - 2*i*p*j + j*j*p
=>  f[k] + k*k*p - (f[j] + j*j*p) < 2*i*p*(k - j)
由于k - j > 0,

    (f[k] + k*k*p - (f[j] + j*j*p)) / (k - j) < 2*i*p

j对应二维点(j, f[j] + j*j*p), k对应二维点(k, f[k] + k*k*p),
连接j, k的直线斜率 < 2*i*p <=> k优于j
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005
long long dp[MAXN][2];
int queue[MAXN];
int head, tail;
long long p;

double slope(long long j, long long k) {
    return ((dp[k][0] + k*k*p - dp[j][0] - j*j*p)*1.0) / (k - j);
}
long long min(long long a, long long b) { return a < b ? a : b; }
int main() {
    int n, q;
    long long i, j;
    scanf("%d %lld %d", &n, &p, &q);
    head = 0, tail = -1;
    queue[head] = 0;
    for (i = 1; i <= n; i++) {
        dp[i][0] = min(dp[i - 1][0], dp[i - 1][1]) + q;
        dp[i][1] = i*i*p; //初始化
        double k = 2.0*i*p;
        while (head < tail && slope(queue[head], queue[head + 1]) < k) head++;
        j = queue[head];
        dp[i][1] = min(dp[i][1], dp[j][0] + (i - j)*(i - j)*p);
        while (head < tail && slope(queue[tail - 1], queue[tail]) > slope(queue[tail], i)) tail--;
        queue[++tail] = i;
    }
    printf("%lld\n", min(dp[n][0], dp[n][1]));
    exit(0);
}

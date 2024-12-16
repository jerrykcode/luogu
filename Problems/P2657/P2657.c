#include "stdio.h"
#include "stdlib.h"
#define N 11
long long dp[N][10];
int val[N];
static inline void init() {
    for (int i = 0; i < 10; i++) dp[1][i] = 1;
    for (int i = 2; i < N; i++)
        for (int j = 0; j < 10; j++) 
            for (int k = 0; k < 10; k++)
                if (abs(j - k) >= 2) dp[i][j] += dp[i - 1][k];
}
long long solve(int x) {
    long long ans = 0ll;
    int len = 0;
    for (int t = x; t; t /= 10) val[++len] = t % 10;
    for (int i = 1; i < len; i++) 
        for (int j = 1; j < 10; j++)
            ans += dp[i][j];
    for (int i = 1; i < val[len]; i++)
        ans += dp[len][i];
    for (int i = len - 1; i >= 1; i--) {
        for (int j = 0; j < val[i]; j++) 
            if (abs(j - val[i + 1]) >= 2) ans += dp[i][j];
        if (abs(val[i + 1] - val[i]) < 2) break;
    }
    return ans;
}
int main() {
    init();
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%lld", solve(b + 1) - solve(a));
    exit(0);
}

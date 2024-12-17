#include "stdio.h"
#include "stdlib.h"
#define N 19
long long dp[N][10];
long long ten[N];
int val[N];
#define MOD 1000000007
static inline void init() {
    for (int i = 0; i <= 9; i++) dp[1][i] = i;
    ten[0] = 1, ten[1] = 10;
    for (int len = 2; len < N; len++) {
        ten[len] = 10 * ten[len - 1];
        for (int i = 0; i <= 9; i++) {
            for (int j = 0; j <= 9; j++, dp[len][i] %= MOD)
                dp[len][i] += dp[len - 1][j];
            dp[len][i] += i * ten[len - 1] % MOD;
            dp[len][i] %= MOD;
        }
    }
}
static inline long long solve(long long x) {
    long long ans = 0ll;
    int len = 0;
    while (x) {
        val[++len] = x % 10;
        x /= 10;
    }
    for (int i = 1; i < len; i++)
        for (int j = 1; j <= 9; j++, ans %= MOD) 
            ans += dp[i][j];
    for (int i = 1; i < val[len]; i++, ans %= MOD) 
        ans += dp[len][i];
    for (int i = len - 1, pre = val[len]; i >= 1; pre += val[i], i--)
        for (int j = 0; j < val[i]; j++, ans %= MOD)
            ans += dp[i][j] + ten[i - 1] * pre % MOD;
    return ans;
}
int main() {
    init();
    int t;
    scanf("%d", &t);
    long long l, r;
    while (t--) {
        scanf("%lld %lld", &l, &r);
        printf("%lld\n", (solve(r + 1) + MOD - solve(l)) % MOD);
    }
    exit(0);
}

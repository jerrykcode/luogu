#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 55

typedef long long ll;

ll dp[N];
char s[N][N];

static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, i, j, k;
    ll ans;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%s", s[i]);
    }
    qsort(s, n, N, strcmp);
    for (i = 0; i < n; i++) {
        dp[i] = 1ll;
        for (j = 0; j < i; j++)
            for (k = min(strlen(s[i]), strlen(s[j])) - 1; k >= 0; k--)
                if (s[i][k] != s[j][k]) {
                    dp[i] += dp[j];
                    break;
                }
    }
    ans = 1ll;
    for (i = 0; i < n; i++)
        ans += dp[i];
    printf("%lld", ans);
    exit(0);
}

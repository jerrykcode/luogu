#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define N 1000006

typedef long long ll;
typedef long double ld;

ll dp[N];
int c[N], p[N], x[N];
ll psum[N], asum[N];
int queue[N], head, tail;

static inline ld slope(int j, int k) {
    if (psum[j] == psum[k]) return DBL_MAX;
    return (dp[j] + asum[j] - dp[k] - asum[k]) * 1.00 / (psum[j] - psum[k]);
}

static inline ll min(ll a, ll b) { return a < b ? a : b; }

int main() {
    int n, i, j;
    scanf("%d", &n);
    for (i = 1; i <= n; i++) {
        scanf("%d %d %d", &x[i], &p[i], &c[i]);
        psum[i] = psum[i - 1] + p[i];
        asum[i] = asum[i - 1] + (ll)p[i] * (ll)x[i];
    }
    dp[0] = 0, dp[1] = c[1];
    queue[0] = 0, queue[1] = 1;
    head = 0, tail = 1;
    ld k;
    for (i = 2; i <= n; i++) {
        k = x[i] * 1.00;
        while (head < tail && slope(queue[head], queue[head+1]) < k) head++;
        j = queue[head];
        dp[i] = dp[j] + c[i] + (psum[i - 1] - psum[j])*x[i] - asum[i - 1] + asum[j];
        while (head < tail && slope(queue[tail - 1], queue[tail]) >= slope(queue[tail], i)) tail--;
        queue[++tail] = i;
    }
    ll ans = dp[n];
    while (p[n] == 0) ans = min(ans, dp[--n]);
    printf("%lld", ans);
    exit(0);
}

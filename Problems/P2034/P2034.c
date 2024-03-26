#include <stdio.h>
#include <stdlib.h>

#define N 100000

struct {
    int idx;
    long long val;
} q[N];
int head, tail, ttl;

static inline void initq(int _ttl) {
    head = 0, tail = -1;
    ttl = _ttl;
}

static inline void pushq(int idx, long long val) {
    while (head <= tail && q[tail].val >= val) tail--;
    q[++tail].idx = idx;
    q[tail].val = val;
}

static inline long long topq() {
    return q[head].val;
}

static inline void checkExpireq(int cur) {
    while (head <= tail && q[head].idx + ttl < cur) head++;
}

long long dp[N];

int main() {
    int n, k, a, i;
    long long ans, tmp;
    scanf("%d %d", &n, &k);
    initq(k + 1);
    for (i = 0; i <= k; i++) {
        scanf("%d", &a);
        ans += a;
        dp[i] = a;
        pushq(i, a);
    }
    for (i = k + 1; i < n; i++) {
        scanf("%d", &a);
        ans += a;
        checkExpireq(i);
        dp[i] = topq() + a;
        pushq(i, dp[i]);
    }
    tmp = dp[n - k - (n > k)];
    for (i = n - k; i < n; i++)
        if (dp[i] < tmp) tmp = dp[i];
    ans -= tmp;
    printf("%lld", ans);
    exit(0);
}

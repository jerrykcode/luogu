#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

ll Seed;
static inline int rnd () {
	static const int MOD = 1e9;
	return Seed = ( 1LL * Seed * 0x66CCFF % MOD + 20120712 ) % MOD;
}

#define N 10000007

struct node {
    ll val;
    int idx;
} q[N];

int x, head, tail;

static inline void init() {
    head = 0, tail = -1;
}

static inline void push(ll val, int idx) {
    while (head <= tail && q[tail].val >= val) tail--;
    q[++tail].val = val;
    q[tail].idx = idx;
}

static inline void expire(int idx) {
    //idx - x is ok
    if (q[head].idx < idx - x) head++;
}

static inline ll top() {
    return q[head].val;
}

static inline void getA(int tp, ll *a) {
    if (tp == 0) {
        scanf("%lld", a);
    } else if (tp == 1) {
        *a = rnd();
    }
}

int main() {
    int n, k, d, tp, i;
    ll a, ans;
    init();
    scanf("%d %d %d %d %d", &n, &k, &d, &x, &tp);
    if (tp == 1) scanf("%lld", &Seed);
    getA(tp, &a);
    ans = a; //dp[0] = a[0]
    push(a, 0); // dp[0] - 0*d
    for (i = 1; i < n; i++) {
        getA(tp, &a);
	// dp[i] = min{ i - x <= j < i | dp[j] + a[i] + k + (i - j - 1)*d }
	//                               dp[j] - j*d + a[i] + k + (i - 1)*d
	expire(i);
	ans = top() + a + k*1ll + (i - 1ll)*d; //dp[i]
        push(ans - (ll)i * (ll)d, i);
    }
    printf("%lld", ans);
}

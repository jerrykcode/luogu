#include <stdio.h>
#include <stdlib.h>

#define N 1000006

int bit[N], n;

static inline int lowbit(int x) { return x & -x; }

static inline void update(int idx, int incr) {
    for ( ; idx <= n; idx += lowbit(idx))
        bit[idx] += incr;
}

static inline int sum(int idx) {
    int sum = 0;
    for ( ; idx >= 1; idx -= lowbit(idx))
        sum += bit[idx];
    return sum;
}

static inline int max(int a, int b) { return a > b ? a : b; }

int main() {
    int m, i, x, y, z, ans = 0;
    scanf("%d %d", &n, &m);
    for (i = 0; i < m; i++) {
        scanf("%d %d %d", &x, &y, &z);
        if (x == y) 
            continue;
        else if (x < y) {
            // [x, y-1]
            update(x, z);
            update(y, -z);
        } else {
            // [x, n]
            // [1, y - 1]
            update(x, z);
            //update(n + 1, -z); // No need to call
            update(1, z);
            update(y, -z);
        }
    }
    for (i = 1; i <= n; i++)
        ans = max(ans, sum(i));
    ans = ans / 36 +  (ans % 36 ? 1 : 0);
    printf("%d", ans);
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>

#define M 100005
#define NAB 11

int dp[M];
int num[NAB][NAB];

struct node {
    int val;
    int idx;
}q[M];
int head, tail, expire;

static inline void init(int _expire) { head = 0, tail = -1, expire = _expire; }

static inline void push(int val, int idx) {
    while (head <= tail && q[tail].val <= val) tail--;
    q[++tail].val = val;
    q[tail].idx = idx;
}

static inline void checkExpire(int idx) {
    if (head <= tail && q[head].idx + expire < idx) head++;
}

static inline int top() { return q[head].val; }

int main() {
    int n, m, k, i, a, b, free = 0, j;
    scanf("%d %d %d", &n, &m, &k);
    for (i = 0; i < n; i++) {
        scanf("%d %d", &a, &b);
        if (b == 0) continue;
        if (a == 0)
            free += b;
        else num[a][b]++;
    }
    for (a = 1; a < NAB; a++)
        for (b = 1; b < NAB; b++) {
            if (num[a][b] == 0) continue;
            for (i = 0; i < a; i++) {
                init(num[a][b]);
                for (j = 0; j <= (m - i) / a; j++) {
                    push(dp[i + j * a] - b * j, j);
                    checkExpire(j);
                    dp[i + j * a] = top() + b * j;
                }
            }
        }
    dp[m] += free;
    printf("%s\n%d", (dp[m] >= k ? "yes" : "no"), dp[m]);
    exit(0);
}

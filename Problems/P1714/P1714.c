#include <stdio.h>
#include <stdlib.h>

#define N 500005
int a[N], sum[N], dp[N];

struct node {
    int val;
    int idx;
} q[N];
int head, tail;

static void init() { head = 0, tail = -1; }

static int pop() { return q[head].val; }

static void check(int expire, int idx) {
    if (q[head].idx + expire < idx) head++;
}

static void push(int val, int idx) {
    while (head <= tail && q[tail].val >= val) tail--;
    q[++tail].val = val;
    q[tail].idx = idx;
}

int main() {
    int n, m, i, ans;
    scanf("%d %d", &n, &m);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        sum[i] = a[i] + (i ? sum[i-1] : 0);
    }
    init();
    ans = 0;
    push(0, -1);
    for (i = 0; i < n; i++) {
        check(m, i);        
        dp[i] = sum[i] - pop();
        push(sum[i], i);
        if (dp[i] > ans) ans = dp[i];
    }
    printf("%d", ans);
    exit(0);
}

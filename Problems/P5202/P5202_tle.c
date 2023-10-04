#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 300005

char s[N];
int h[N], g[N];
int dp[N];
struct {
    int idx;
    int dpval;
} q[N];
int head, tail;

static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, k, i, seg, j, tmp, ans;
    scanf("%d %d", &n, &k);
    scanf("%s", s);
    n = strlen(s);
    for (i = 0; i < n; i++) {
        h[i] = (s[i] == 'H' ? 1 : 0) + (i > 0 ? h[i - 1] : 0);
        g[i] = (s[i] == 'G' ? 1 : 0) + (i > 0 ? g[i - 1] : 0);
    }
    for (i = 0; i < k; i++)
        dp[i] = g[i] >= h[i] ? 1 : 0;
    for (; i < n; i++) dp[i] = -1;
    if (dp[n - 1] >= 0) { ans = dp[n - 1]; goto END; }
    ans = 0x7fffffff;
    for (seg = 2; seg <= n; seg++) {
        q[(head = tail = 0)].idx = seg - 2;
        q[head].dpval = dp[seg - 2];
        for (i = seg - 1; i < n; i++) {
            if (q[head].idx + k < i) head++;
            if (head > tail) break;
            j = q[head].idx;
            tmp = dp[i];
            dp[i] = q[head].dpval + (g[i] - g[j] >= h[i] - h[j] ? 1 : 0);
            if (tmp >= 0) {
                while (head <= tail 
                        && (q[tail].dpval > tmp 
                            || (q[tail].dpval == tmp && g[i] - g[q[tail].idx] >= h[i] - h[q[tail].idx]) )) tail--;
                q[++tail].idx = i;
                q[tail].dpval = tmp;
            }
        }
        if (dp[n - 1] >= 0) ans = min(ans, dp[n - 1]);
    }
END:
    printf("%d", ans);
    exit(0);
}

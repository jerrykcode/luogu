#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 300005

char s[N];
int h[N], g[N];
int dp[N];
int q[N], head, tail;

int main() {
    int n, k, i;
    scanf("%d %d", &n, &k);
    scanf("%s", s);
    n = strlen(s);
    for (i = 0; i < n; i++) {
        h[i] = (s[i] == 'H' ? 1 : 0) + (i ? h[i - 1] : 0);
        g[i] = (s[i] == 'G' ? 1 : 0) + (i ? g[i - 1] : 0);
    }
    dp[0] = s[0] == 'G' ? 1 : 0;
    q[(head = tail = 0)] = 0;
    for (i = 1; i < n; i++) {
        if (i < k)  {
            dp[i] = g[i] >= h[i] ? 1 : 0;
        } else {
            while (head <= tail && q[head] + k < i) head++;
            dp[i] = dp[q[head]] + (g[i] - g[q[head]] >= h[i] - h[q[head]] ? 1 : 0);
        }
        while (head <= tail
                && (dp[q[tail]] > dp[i]
                    || (dp[q[tail]] == dp[i] && g[i] - g[q[tail]] >= h[i] - h[q[tail]])
                   ) 
              ) tail--;
        q[++tail] = i;
    }
    printf("%d\n", dp[n - 1]);
    exit(0);
}

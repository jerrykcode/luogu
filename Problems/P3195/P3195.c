/*
题目地址:  https://www.luogu.com.cn/problem/P3195
视频讲解:  https://www.bilibili.com/video/BV1Fh411d7jA

草稿
sum[i] = sum[i - 1] + c[i]

dp[i] = min{dp[j] + (sum[i] - sum[j] + i - j - 1 - l)^2}
L = l + 1
s[i] = sum[i] + i
s[j] = sum[j] + j
dp[i] = min{dp[j] + (s[i] - s[j] - L)^2}

j1 < j2 < i

    dp[j2] + (s[i] - s[j2] - L)^2 < dp[j1] + (s[i] - s[j1] - L)^2
=>  dp[j2] + (s[i] - L)^2 - 2*(s[i] - L)*s[j2] + s[j2]^2 < dp[j1] + (s[i] - L)^2 - 2*(s[i] - L)*s[j1] + s[j1]^2
=>  dp[j2] + s[j2]^2 - 2*(s[i] - L)*s[j2] < dp[j1] + s[j1]^2 - 2*(s[i] - L)*s[j1]
=>  dp[j2] + s[j2]^2 - dp[j1] - s[j1]^2 < 2*(s[i] - L)*s[j2] - 2*(s[i] - L)*s[j1]
=>  dp[j2] + s[j2]^2 - dp[j1] - s[j1]^2 < 2*(s[i] - L)*(s[j2] - s[j1])     #s[j2] - s[j1] > 0
=>  (dp[j2] + s[j2]^2 - dp[j1] - s[j1]^2) / (s[j2] - s[j1]) < 2*(s[i] - L)


y(j) = s[j]
x(j) = dp[j] + s[j]^2

2*(s[i] - L)

*/

#include <stdio.h>
#include <stdlib.h>

typedef long long ll;
#define maxn 50000
ll sum[maxn], s[maxn], dp[maxn];
int queue[maxn], head, tail;
#define pow(a) ((a)*(a))
double slop(int i, int j) {
    double yi = dp[i] + pow(s[i]);
    double yj = dp[j] + pow(s[j]);
    return (yj - yi) / (s[j] - s[i]);
}

ll min(ll a, ll b) { return a < b ? a : b; }

int main() {
    int n, l, L;
    scanf("%d %d", &n, &l);
    L = l + 1;
    head = 0; tail = -1;
    for (int i = 0; i < n; i++) {
        scanf("%lld", sum + i);
        if (i) sum[i] += sum[i - 1];
        s[i] = sum[i] + i;
        dp[i] = pow(sum[i] + i - l); //init
        double t = 2.0 * (s[i] - L);
        while (head < tail && slop(queue[head], queue[head + 1]) < t) head++;
        int top = queue[head];
        dp[i] = min(dp[i], dp[top] + pow(s[i] - s[top] - L));
        while (head < tail && slop(queue[tail - 1], queue[tail]) >= slop(queue[tail], i)) tail--;
        queue[++tail] = i;
    }
    printf("%lld\n", dp[n - 1]);
    exit(0);
}

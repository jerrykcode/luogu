#include <stdio.h>
#include <stdlib.h>

#define N 100005

int a[N];
long long sum[N];
int l[N], r[N];

static inline long long max(long long a, long long b) { return a > b ? a : b; }

int main() {
    int n, i, start, end;
    long long ans = 0ll;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        sum[i] = a[i] * 1ll;
        if (i) sum[i] += sum[i - 1];
    }
    for (i = 0; i < n; i++) {
        start = i - 1;
        while (start >= 0 && a[start] >= a[i]) start = l[start] - 1;
        l[i] =  ++start;
    }
    for (i = n - 1; i >= 0; i--) {
        end = i + 1;
        while (end < n && a[end] >= a[i]) end = r[end] + 1;
        r[i] = --end;
    }
    for (i = 0; i < n; i++)
        ans = max(ans, (sum[r[i]] - (l[i] ? sum[l[i] - 1] : 0)) * 1ll * a[i]);
    printf("%lld", ans);
    exit(0);
}

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define N 100005

char s[N];
int sa[N], id[N], rk[N << 1], oldrk[N << 1], cnt[N], height[N];
int n, m, i, j, k, w, p;

typedef long long ll;
ll ans;

void build_sa() {
    m = 26;
    for (i = 1; i <= n; i++) ++cnt[rk[i] = s[i] - 'a' + 1];
    for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
    for (i = n; i >= 1; i--) sa[cnt[rk[i]]--] = i;
    for (w = 1; w < n; w <<= 1, m = p) {
        memset(cnt, 0, sizeof(cnt));
        for (i = 1; i <= n; i++) id[i] = sa[i];
        for (i = 1; i <= n; i++) ++cnt[rk[id[i] + w]];
        for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
        for (i = n; i >= 1; i--) sa[cnt[rk[id[i]+w]]--] = id[i];
        memset(cnt, 0, sizeof(cnt));
        for (i = 1; i <= n; i++) id[i] = sa[i];
        for (i = 1; i <= n; i++) ++cnt[rk[id[i]]];
        for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
        for (i = n; i >= 1; i--) sa[cnt[rk[id[i]]]--] = id[i];
        memcpy(oldrk, rk, sizeof(rk));
        for (p = 0, i = 1; i <= n; i++) {
            if (oldrk[sa[i]] == oldrk[sa[i - 1]] && oldrk[sa[i] + w] == oldrk[sa[i - 1] + w])
                rk[sa[i]] = p;
            else rk[sa[i]] = ++p;
        }
    }
}

int max(int a, int b) { return a > b ? a : b; }

void build_height() {
    for (i = 1; i <= n; i++) {
        if (rk[i] == 1) continue;
        j = sa[rk[i] - 1];
        k = max(0, height[rk[i - 1]] - 1);
        while (s[i + k] == s[j + k]) k++;
        height[rk[i]] = k;
        ans -= 1ll*k;
    }
}

int main() {
    scanf("%d", &n);
    scanf("%s", s + 1);
    ans = 1ll*n*(n+1)/2;
    build_sa();
    build_height();
    printf("%lld\n", ans);
    exit(0);
}

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAXN 1000006

char s[MAXN];
int sa[MAXN], rk[MAXN << 1], oldrk[MAXN << 1], id[MAXN], cnt[MAXN];

int main() {
    scanf("%s", s + 1);
    int n = strlen(s + 1);
    int m = 300;
    int i, w, p;
    for (i = 1; i <= n; i++) ++cnt[rk[i] = s[i]];
    for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
    for (i = n; i >= 1; i--) sa[cnt[rk[i]]--] = i;

    for (w = 1; w < n; w <<= 1, m = p) {
        memset(cnt, 0, sizeof(cnt));
        for (i = 1; i <= n; i++) id[i] = sa[i];
        for (i = 1; i <= n; i++) ++cnt[rk[id[i] + w]];
        for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
        for (i = n; i >= 1; i--) sa[cnt[rk[id[i] + w]]--] = id[i];

        memset(cnt, 0, sizeof(cnt));
        for (i = 1; i <= n; i++) id[i] = sa[i];
        for (i = 1; i <= n; i++) ++cnt[rk[id[i]]];
        for (i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
        for (i = n; i >= 1; i--) sa[cnt[rk[id[i]]]--] = id[i];

        memcpy(oldrk, rk, sizeof(rk));
        for (p = 0, i = 1; i <= n; i++) {
            if (oldrk[sa[i]] == oldrk[sa[i - 1]] && oldrk[sa[i] + w] == oldrk[sa[i - 1] + w])
                rk[sa[i]] = p;
            else
                rk[sa[i]] = ++p;
        }
    }
    for (int i = 1; i < n; i++) printf("%d ", sa[i]); printf("%d\n", sa[n]);
    exit(0);
}

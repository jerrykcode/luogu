#include <stdio.h>
#include <stdlib.h>

#define N 1000006

int k[N], id[N];

int cmp(const void *a, const void *b) {
    return k[*((int *)b)] - k[*((int *)a)];
}

#define max(a, b) ((a) > (b) ? (a) : (b))

int main() {
    int n, i;
    scanf("%d", &n);
    for (i = 1; i <= n; i++) {
        scanf("%d", &k[i]);
        id[i] = i;
    }
    qsort(id + 1, n, sizeof(int), cmp);
    long long ans = 0ll;
    int maxId = 0;
    for (i = 1; i <= n; i++) {
        ans = max(ans, 1ll*k[id[i]]*(id[i] + maxId));
        maxId = max(maxId, id[i]);
    }
    printf("%lld", ans);
    exit(0);
}

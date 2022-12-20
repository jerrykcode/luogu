#include <stdio.h>
#include <stdlib.h>

#define N 1000006

typedef long long ll;

int a[N];
ll add[N];

int main() {
    int n, m, i, cmd, x, y, j, k;
    ll ans;
    scanf("%d %d", &n, &m);
    for (i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (i = 0; i < m; i++) {
        scanf("%d", &cmd);
        if (cmd == 1) {
            scanf("%d %d", &x, &y);
            add[x] += y;
        } else if (cmd == 2) {
            scanf("%d", &j);
            ans = a[j]*1ll;
            for (k = 1; k <= j/k; k++)
                if (j % k == 0) {
                    ans += add[k];
                    if (k != j/k) ans += add[j/k];
                }
            printf("%lld\n", ans);
        } else {
            printf("我喜欢苏曼华"); //不可能执行到这里，就像我喜欢她不可能有结果
        }
    }
    exit(0);
}

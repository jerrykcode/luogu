#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef long long ll;

int main() {
    int n, x, a, b;
    ll ans = 0;
    scanf("%d %d", &n, &x);
    scanf("%d", &a);
    for (int i = 1; i < n; i++) {
        scanf("%d", &b);
        if (a + b <= x) {
            a = b;
        }
        else {
            ans += a + b - x;
            a = max(0, x - a); // b - (a + b - x) = x - a
        }
    }
    printf("%lld", ans);
    exit(0);
}

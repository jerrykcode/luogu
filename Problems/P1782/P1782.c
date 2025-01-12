#include "stdio.h"
#include "stdlib.h"
#define N 10004
#define M 6
#define C 10004
long long f[C];
struct qele {
    int val;
    int idx;
} q[C];
int qh, qt;
int main() {
    int n, m, c;
    scanf("%d %d %d", &n, &m, &c);
    for (int i = 0, v, w, d, j, k, val; i < n; i++) {
        scanf("%d %d %d", &v, &w, &d);
        for (j = 0; j < v; j++) {
            qh = 0, qt = -1;
            for (k = 0; k <= (c - j) / v; k++) {
                val = f[j + k * v] - k * w;
                while (qh <= qt && q[qt].val < val) qt--;
                q[++qt] = (struct qele){val, k};
                while (qh <= qt && q[qh].idx + d < k) qh++;
                f[j + k * v] = q[qh].val + k * w;
            }
        }
    }
    for (int i = 0, a, b, cc, j; i < m; i++) {
        long long x, val;
        scanf("%d %d %d", &a, &b, &cc);
        for (j = c; j >= 0; j--)
            for (x = 0ll; x <= j; x++) {
                val = f[j - x] + a*x*x + b*x + cc;
                if (val > f[j]) f[j] = val;
            }
    }
    printf("%lld", f[c]);
    exit(0);
}

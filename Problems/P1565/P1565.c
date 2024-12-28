#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define N 202
long long a[N][N];
long long h[N];
static inline long long max(long long x, long long y) { return x > y ? x : y; }
static inline long long min(long long x, long long y) { return x < y ? x : y; }
int main() {
    int n, m, ans = 0;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) scanf("%lld", &a[i][j]);
    for (int i = 0; i < n; i++) {
        memset(h, 0, sizeof(h));
        for (int j = i; j < n; j++) {
            int left = 1, right = n, mid, len = 0;
            while (left <= right) {
                mid = (left + right) >> 1;
                long long maxsegsum = -114514ll, cursum = 0ll, curavlsum = 0ll, minsum = 0ll;
                for (int k = 0; k < m; k++) {
                    h[k] += a[j][k];
                    cursum += h[k];
                    if (k >= mid - 1) {
                        maxsegsum = max(maxsegsum, cursum - minsum);
                        curavlsum += h[k - (mid - 1)];
                        minsum = min(minsum, curavlsum);
                    }
                }
                if (maxsegsum > 0ll) {
                    len = mid;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            ans = max(ans, len * (j - i + 1));
        }
    }
    printf("%d\n", ans);
    exit(0);
}

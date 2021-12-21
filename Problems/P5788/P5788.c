#include "stdio.h"
#include "stdlib.h"

#define N 3000006

int a[N], q[N], f[N];

int main() {
    int n, i, r;
    scanf("%d", &n);
    for (i = 0; i < n; i++) scanf("%d", &a[i]);
    r = 0;
    for (i = n - 1; i >= 0; i--) {
        while (r && a[i] >= a[q[r]]) r--;
        if (r)
            f[i] = q[r] + 1; //从0开始转为从1开始
        else f[i] = 0;
        q[++r] = i;
    }
    for (i = 0; i < n - 1; i++) printf("%d ", f[i]); printf("%d", f[n - 1]);
    exit(0);
}

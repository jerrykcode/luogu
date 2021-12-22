#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define N 1001
int p[N];

int find(int a) {
    if (p[a] < 0) return a;
    return p[a] = find(p[a]);
}

int merge(int a, int b) {
    int ra = find(a);
    int rb = find(b);
    if (ra == rb)
        return 0;
    if (p[ra] < p[rb]) {
        p[ra] += p[rb];
        p[rb] = ra;
    }
    else {
        p[rb] += p[ra];
        p[ra] = rb;
    }
    return 1;
}

int main() {
    int n, m, i, a, b;
    scanf("%d", &n);
    while (n) {
        memset(p, -1, n*4);
        scanf("%d", &m);
        for (i = 0; i < m; i++) {
            scanf("%d %d", &a, &b);
            a--; b--;
            if (merge(a, b)) n--;
        }
        printf("%d\n", n - 1);
        scanf("%d", &n);
    }
    exit(0);
}

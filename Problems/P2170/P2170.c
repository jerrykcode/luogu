#include <stdio.h>
#include <stdlib.h>

#define maxn 20000
#define maxm 20000

short uset[maxn];
short dp[maxm];

void Init(int n) {
    for (int i = 0; i < n; i++)
        uset[i] = -1;
}

int FindRoot(int a) {
    if (uset[a] < 0) return a;
    return (uset[a] = FindRoot(uset[a]));
}

void Union(int a, int b) {
    int ra = FindRoot(a), rb = FindRoot(b);
    if (ra == rb) return;
    if (uset[ra] < uset[rb]) {
        uset[ra] += uset[rb];
        uset[rb] = ra;
    }
    else {
        uset[rb] += uset[ra];
        uset[ra] = rb;
    }
}

int Closer(int a, int b, int target) {
    int at = abs(a - target);
    int bt = abs(b - target);
    return at == bt ? (a < b ? a : b) : (at < bt ? a : b);
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    Init(n);
    int a, b;
    for (int i = 0; i < k; i++) {
        scanf("%d %d", &a, &b);
        a--; b--;
        Union(a, b);
    }
    //DP
    int i, j, tmp;
    for (i = 0; i < n; i++) {
        if (uset[i] >= 0) continue;
        tmp = -uset[i];
        for (j = m; j > tmp; j--)
            dp[j] = Closer(dp[j], dp[j - tmp] + tmp, j);
        for (j = tmp; j > 0; j--)
            dp[j] = Closer(dp[j], tmp, j);
    }
    printf("%hd\n", dp[m]);
    exit(0);
}

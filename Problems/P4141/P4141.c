#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define maxn 2000
#define maxm 2000

int w[maxn];
int f[maxm + 1];
int g[maxm + 1];

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++)
        scanf("%d", &w[i]);
    f[0] = 1;
    for (int i = 0; i < n; i++) 
        for (int j = m; j >= w[i]; j--) {
            f[j] = (f[j] + f[j - w[i]]) % 10;
        }
    for (int i = 0; i < n; i++) {
        g[0] = 1;
        for (int j = 1; j <= m; j++) {
            if (j < w[i]) g[j] = f[j] % 10;
            else g[j] = (f[j] - g[j - w[i]] + 10) % 10;
            printf("%d", g[j]);
        }
        printf("\n");
    }
    return 0;
}

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define maxn 1000000
#define GREEN 'G'
#define RED 'R'

#define NOT_A_LOC 0xffffffff
int loc[2 * maxn + 2];
int sum[maxn + 1];
char a[maxn + 1];

int hash(int index) { return index + maxn; }
int max(int a, int b) { return a > b ? a : b; }

int main() {
    scanf("%s", a + 1);    
    int n = strlen(a + 1);
    int res = 0;
    memset(loc, NOT_A_LOC, sizeof(loc));
    loc[hash(0)] = 0;
    for (int i = 1; i <= n; i++) {
        if (i) sum[i] = sum[i - 1];
        if (a[i] == GREEN) sum[i] += 1;
        else sum[i] -= 1;
        if (loc[hash(sum[i])] != NOT_A_LOC) {
            res = max(res, i - loc[hash(sum[i])]);
        }
        else loc[hash(sum[i])] = i;
    }
    printf("%d", res);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#define N 101

int presum[N<<1];
int dpmin[N<<1][N<<1];
int dpmax[N<<1][N<<1];

#define RANGE_VAL(start, end) (start > 0 ? \
                                 presum[end] - presum[(start) - 1]\
                                :presum[end])
#define ARRAY_VAL(idx) RANGE_VAL(idx, idx)

#define SIGNED_INT32_MAX 0x7fffffff

static int max(int a, int b) { return a > b ? a : b; }
static int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &presum[i]);
        if (i) presum[i] += presum[i - 1];
    }
/*    if (n == 1){
        printf("0\n0");
        exit(0);
    }*/
    for (int i = n; i < n<<1; i++) {
        presum[i] = presum[i - 1] + ARRAY_VAL(i - n);
    }
    for (int len = 2; len <= n; len++) {
        for (int start = 0, end = len - 1; end < n<<1; start++, end++) {
            dpmax[start][end] = 0;
            dpmin[start][end] = SIGNED_INT32_MAX;
            for (int i = start; i < end; i++) {
                dpmax[start][end] = max(dpmax[start][end], dpmax[start][i] + dpmax[i + 1][end]);
                dpmin[start][end] = min(dpmin[start][end], dpmin[start][i] + dpmin[i + 1][end]);
            }
            dpmax[start][end] += RANGE_VAL(start, end);
            dpmin[start][end] += RANGE_VAL(start, end);
        }
    }
    int resmax = 0, resmin = SIGNED_INT32_MAX;
    for (int start = 0, end = n - 1; end < n<<1; start++, end++) {
        resmax = max(resmax, dpmax[start][end]);
        resmin = min(resmin, dpmin[start][end]);
    }
    printf("%d\n%d", resmin, resmax);
    exit(0);
}

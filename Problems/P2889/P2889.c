#include <stdio.h>
#include <stdlib.h>

#define maxm 1000

typedef struct interval {
    int start, end;
    int eff;
} interval;

int compare(const void *a, const void *b) {
    return ((interval *)a)->end - ((interval *)b)->end;
}

interval ins[maxm];
int dp[maxm];

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n, m, r;
    scanf("%d %d %d", &n, &m, &r);
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &ins[i].start, &ins[i].end, &ins[i].eff);
    }
    qsort(ins, m, sizeof(interval), compare);
    dp[0] = ins[0].eff;
    int left, right, mid, k;
    for (int i = 1; i < m; i++) {
        //binary search
        left = 0, right = i;
        k = -1;
        while (left <= right) {
            mid = (left + right) >> 1;
            if (ins[mid].end <= ins[i].start - r) {
                k = mid;
                left = mid + 1;
            }
            else right = mid - 1;
        }        
        //DP
        dp[i] = max(dp[i - 1], (k != -1 ? dp[k] : 0) + ins[i].eff);
    }
    printf("%d\n", dp[m - 1]);
    exit(0);
}

#include "stdio.h"
#include "stdlib.h"

#define maxn 150000

struct interval {
    int start, end;
} intervals[maxn];

int len(int idx) {
    return intervals[idx].end - intervals[idx].start + 1;
}

int compare(const void * a, const void *b) {
    return ((struct interval *)a)->end - ((struct interval *)b)->end;
}

int dp[maxn];

int binary_search(int idx) {
    int left = 0, right = idx, mid;
    int result = -1;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (intervals[mid].end < intervals[idx].start) {
            result = mid;
            left = mid + 1;
        }
        else right = mid - 1;
    }
    return result;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d %d", &intervals[i].start, &intervals[i].end);
    qsort(intervals, n, sizeof(struct interval), compare);
    dp[0] = len(0);
    int last;
    for (int i = 1; i < n; i++) {
        last = binary_search(i);
        if (last == -1)
            dp[i] = len(i);
        else
            dp[i] = dp[last] + len(i);
        dp[i] = max(dp[i - 1], dp[i]);
    }
    printf("%d", dp[n - 1]);
    return 0;
}

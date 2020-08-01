#include <stdio.h>
#include <stdlib.h>

#define maxk 30000

struct Segment {
    short start, end;
} segments[maxk];

int compare(const void *a, const void *b) {
    return ((struct Segment *)a)->end - ((struct Segment *)b)->end;
}

short dp[maxk];

short max(short a, short b) {
    return a > b ? a : b;
}

short len(struct Segment segment) {
    return segment.end - segment.start;// + 1;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%hd %hd", &segments[i].start, &segments[i].end);
    }
    qsort(segments, n, sizeof(struct Segment), compare);
    for (int i = 0; i < n; i++) {
        dp[segments[i].end] = max(dp[segments[i].end], dp[segments[i].start] + len(segments[i]));
        if (i == n - 1) break;
        for (int j = segments[i].end + 1; j <= segments[i + 1].end; j++)
            dp[j] = dp[segments[i].end];
    }
    printf("%hd\n", dp[segments[n - 1].end]);
    return 0;
}

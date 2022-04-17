#include "stdio.h"
#include "stdlib.h"

#define N 2000006

struct node {
    int idx, val;
} s[N];
int head, tail;

static void init() {
    head = 0, tail = -1;
}

static void expire(int idx) {
    if (head <= tail && s[head].idx < idx) head++;
}

static void add(int idx, int val) {
    while (head <= tail && s[tail].val > val) tail--;
    tail++;
    s[tail].idx = idx;
    s[tail].val = val;
}

static int get() {
    return s[head].val;
}

int main() {
    init();
    int n, m, val, pre;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%d", &val);
        if (i) {
            expire(i - m);
            add(i - 1, pre);
        }
        printf("%d\n", get());
        pre = val;
    }
    exit(0);
}

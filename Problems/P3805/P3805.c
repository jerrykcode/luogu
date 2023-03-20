#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 11000007
#define N2 (N<<1)

char raw[N], s[N2];
int p[N2];

static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, i, j, left, right, ans;
    scanf("%s", raw);
    n = strlen(raw);
    s[0] = '$';
    for (i = 0, j = 1; i < n; i++) {
        s[j++] = raw[i];
        s[j++] = '$';
    }
    s[j] = 0;
    n = j;
    left = 0, right = -1, ans = 0;
    for (i = 0; i < n; i++) {
        p[i] = (i >= right ? 0 : min(p[left + right - i], right - i));
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < n && s[i - p[i] - 1] == s[i + p[i] + 1]) p[i]++;
        if (i + p[i] > right) {
            left = i - p[i];
            right = i + p[i];
        }
        if (p[i] > ans) ans = p[i];
    }
    printf("%d", ans);
    exit(0);
}

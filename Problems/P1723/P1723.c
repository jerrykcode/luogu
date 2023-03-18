#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S 2000006

char raw[S>>1], s[S];
int p[S];

static inline int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, slen, i, j, center, right, ans;
    scanf("%d", &n);
    while (n--) {
        scanf("%s", raw);
        s[0] = '%';
        slen = strlen(raw);
        for (i = 0, j = 1; i < slen; i++) {
            s[j++] = raw[i];
            s[j++] = '%';
        }
        s[j] = '\0';
        slen = strlen(s);
        ans = 0;
        memset(p, 0, sizeof(p));
        for (i = 0, center = 0, right = -1; i < slen; i++) {
            j = (i >= right ? 0 : min(p[(center << 1) - i], right - i));
            while (i + j + 1 < slen && i - j - 1 >= 0 && s[i + j + 1] == s[i - j - 1]) j++;
            p[i] = j;
            if (i + j > right) {
                right = i + j;
                center = i;
            }
            if (p[i] > ans) ans = p[i];
        }
        printf("%d\n", ans);
    }
    exit(0);
}

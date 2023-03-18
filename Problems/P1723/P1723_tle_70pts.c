#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 1000000

char s[LEN];

int palindromes[2][LEN];
int psize[2];

static inline int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, slen, cur, pre, cpsize, i, j, k, ans;
    scanf("%d", &n);
    while (n--) {
        scanf("%s", s);
        slen = strlen(s);
        cur = 0, pre = 1;
        ans = 1;
        psize[pre] = 0;
        for (i = 0; i < slen; i++) {
            cpsize = 0;
            palindromes[cur][cpsize++] = i;
            if (i - 1 >= 0 && s[i - 1] == s[i])
                palindromes[cur][cpsize++] = i - 1;
            for (j = 0; j < psize[pre]; j++) {
                k = palindromes[pre][j];
                if (k - 1 >= 0 && s[k - 1] == s[i])
                    palindromes[cur][cpsize++] = k - 1;
            }
            ans = max(ans, i - palindromes[cur][cpsize - 1] + 1);
            psize[cur] = cpsize;
            cur += pre;
            pre = cur - pre;
            cur -= pre;
        }
        printf("%d\n", ans);
    }
    exit(0);
}

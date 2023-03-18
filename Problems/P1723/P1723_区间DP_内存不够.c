#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LEN 5000

bool dp[LEN][LEN];

char s[LEN];

int main() {
    int n, slen, len, i, j, ans;
    scanf("%d", &n);
    while (n--) {
        scanf("%s", s);
        slen = strlen(s);
        memset(dp, 0, sizeof(dp));  // false
        ans = 1;
        for (i = 0; i < slen; i++) {
            dp[i][i] = true;
            if (i + 1 < slen && s[i] == s[i + 1]) { dp[i][i + 1] = true; ans = 2; }
        }
        for (len = 3; len <= slen; len++) {
            for (i = 0, j = len - 1; j < slen; i++, j++) {
                dp[i][j] = s[i] == s[j] && dp[i + 1][j - 1];
                if (dp[i][j]) ans = len;
            }
        }
        printf("%d\n", ans);
    }
    exit(0);
}

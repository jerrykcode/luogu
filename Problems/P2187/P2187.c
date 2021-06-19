#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN 100000

char s[MAXN];
int dp[MAXN];
char pair[2];
bool invalid[26][26];

int max(int a, int b) { return a > b ? a : b; }

int main() {
    int n, m;
    scanf("%d", &n);
    scanf("%s", s);
    scanf("%d", &m);
    int i, j;
    for (i = 0; i < m; i++) {
        scanf("%s", pair);
        invalid[pair[0] - 'a'][pair[1] - 'a']
            = invalid[pair[1] - 'a'][pair[0] - 'a'] = true;
    }
    //DP
    bool invalid_ij;
    int ans = 0;
    for (i = 0; i < n; i++) {
        dp[i] = 1;
        for (j = i - 1; j >= 0; j--) {
            invalid_ij = invalid[s[i] - 'a'][s[j] - 'a'];
            if (s[i] != s[j]) {
                if (invalid_ij) continue;
                dp[i] = max(dp[i], dp[j] + 1);
            }
            else {
                if (invalid_ij)
                    dp[i] = max(dp[i], dp[j]);
                else
                    dp[i] = max(dp[i], dp[j] + 1);
                break;
            }
        }
        ans = max(ans, dp[i]);
    }
    printf("%d\n", n - ans);
    exit(0);
}

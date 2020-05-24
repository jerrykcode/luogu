#include <stdio.h>
#include <stdlib.h>

#define maxm 10000
int dp[maxm + 1];

int main() {
    int n, m, a;
    fscanf(stdin, "%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        fscanf(stdin, "%d", &a);        
        for (int j = m; j > a; j--)
            dp[j] += dp[j - a];
        dp[a]++;
    }
    fprintf(stdout, "%d", dp[m]);
    return 0;
}

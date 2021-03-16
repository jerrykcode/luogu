#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXN 10
#define MAXS 20001
#define INF 0x7fffffff

int prime[MAXS], numPrimes;
bool b[MAXS];
int dp[MAXS];
int s_arr[MAXN];;

void build_prime(int s) {
    memset(b, true, sizeof(b));
    numPrimes = 0;
    b[0] = b[1] = false;
    for (int i = 2; i < s; i++)
        if (b[i]) {
            prime[numPrimes++] = i;
            for (int j = 2; i * j <= s; j++)
                b[i * j] = false;
        }
}

int min(int a, int b) {
    return a < b ? a : b;
}
int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    int n, s = 0;
    scanf("%d", &n);
    int i, j;    
    for (i = 0; i < n; i++) {
        scanf("%d", &s_arr[i]);
        if (s_arr[i] > s)
            s = s_arr[i];
    }
    build_prime(s);
    memset(b, false, sizeof(b));
    for (i = 2; i <= s; i++)
        for (j = 0; j < numPrimes && prime[j] <= i; j++)
            if (!b[i - prime[j]]) {
                b[i] = true;
                dp[i] = INF; //必胜状态下初始化dp[i]
                break;
            }
    for (i = 2; i <= s; i++)
        for (j = 0; j < numPrimes && prime[j] <= i; j++)
            if (b[i]) {
                if (!b[i - prime[j]])
                    dp[i] = min(dp[i], dp[i - prime[j]] + 1);
            }
            else dp[i] = max(dp[i], dp[i - prime[j]] + 1);
    for (i = 0; i < n; i++)
        printf("%d\n", (b[s_arr[i]] ? dp[s_arr[i]] : -1));
    exit(0);
}

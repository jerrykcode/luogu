#include "stdio.h"
#include "stdlib.h"

#define maxn 9
#define maxk 81
#define maxnum 512

int status[maxnum];
int num_status = 0;
int num_ones[maxnum];
long long dp[maxn][maxnum][maxk + 1];

void init(int n) {    
    for (int i = 0; i < 1 << n; i++) {
        if (i & (i << 1)) continue;
        status[num_status] = i;
        for (int j = 0; j < n; j++)
            if (i & (1 << j)) num_ones[num_status]++;
        num_status++;
    }
}

int main() {
    int n, king;
    scanf("%d %d", &n, &king);
    init(n);
    for (int i = 0; i < num_status; i++)
        dp[0][i][num_ones[i]] = 1;
    for (int row = 1; row < n; row ++)
        for (int i = 0; i < num_status; i++)
            for (int j = 0; j < num_status; j++) {
                if (status[i] & status[j]) continue;
                if ((status[i] << 1) & status[j]) continue;
                if ((status[i] >> 1) & status[j]) continue;
                for (int k = king; k >= num_ones[i]; k--)
                    dp[row][i][k] += dp[row - 1][j][k - num_ones[i]];
            }
    long long res = 0;
    for (int i = 0; i < num_status; i++)
        res += dp[n - 1][i][king];
    printf("%lld", res);
    return 0;
}

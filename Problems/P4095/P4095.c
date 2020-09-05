#include <stdio.h>
#include <stdlib.h>

#define maxn 1000
#define maxe 1000

int dp[2][maxn][maxe + 1];

typedef struct {
    int index;
    int value;
}Node;
Node queue[maxe];
int head, tail;

void Init() {
    head = 0;
    tail = -1;
}

void Push(int index, int value) {
    while (tail >= head && queue[tail].value < value) {
        tail--;
    }
    tail++;
    queue[tail].index = index;
    queue[tail].value = value;
}

void Check(int index) {
    if (queue[head].index < index) head++;
}

int Top() {
    return queue[head].value;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int val[maxn], cost[maxn], num[maxn];

int main() {
    int n, q, d, e, ans;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &cost[i], &val[i], &num[i]); 
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < cost[i]; j++) {
            Init();
            for (int k = 0; k <= (maxe - j) / cost[i]; k++) {
                Push(k, (i ? dp[0][i - 1][k * cost[i] + j] : 0) - k * val[i]);
                Check(k - num[i]);
                dp[0][i][k * cost[i] + j] = Top() + k * val[i];
            }
        }
    for (int i = n - 1; i >= 0; i--)
        for (int j = 0; j < cost[i]; j++) {
            Init();
            for (int k = 0; k <= (maxe - j) / cost[i]; k++) {
                Push(k, (i < n - 1 ? dp[1][i + 1][k * cost[i] + j] : 0) - k * val[i]);
                Check(k - num[i]);
                dp[1][i][k * cost[i] + j] = Top() + k * val[i];
            }
        }    
    scanf("%d", &q);
    for (int i = 0; i < q; i++) {
        scanf("%d %d", &d, &e);
        ans = 0;
        if (d > 0 && d < n - 1) {
            for (int j = 0; j <= e; j++)
                ans = max(ans, dp[0][d - 1][j] + dp[1][d + 1][e - j]);
        }
        else if (d == 0) ans = dp[1][1][e];
        else ans = dp[0][n - 2][e];
        printf("%d\n", ans);
    }
    exit(0);
}

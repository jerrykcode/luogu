#include "stdio.h"
#include "stdlib.h"

#define maxn 100
#define maxk 99
#define inf  0x3fffffff    //注意inf不能设置为0xffffffff, 因为状态转移时有可能把inf加上一个值, 要防止溢出

typedef unsigned char byte;   //书的长, 宽均小于200, unsigned char可以表达[0, 255]

struct Book {
    byte height, width;
} books[maxn + 1];
int compare(const void * a, const void * b) {
    return ((struct Book*)a)->height - ((struct Book*)b)->height;
}

int dp[maxn + 1][maxk + 1][maxn + 1];

int min(int a, int b) { return a < b ? a : b; }
int abs(int a) { return a >= 0 ? a : 0 - a; }

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%hhu %hhu", &books[i].height, &books[i].width);
    }
    qsort(books + 1, n, sizeof(struct Book), compare);
    //初始化
    dp[1][0][1] = 0;
    for (int i = 2; i <= n; i++) {
        for (int j = 0; j <= min(k, i - 2); j++)
            for (int l = i - j; l <= i; l++) dp[i][j][l] = inf;
        /*for (int l = 1; l <= i; l++)
            dp[i][i - 1][l] = 0; //删到只剩一本书, 不整齐度为0, 但是由于dp数组本来就是全0, 所以这里的赋值就不必要了
        */
    }
    //状态转移
    for (int i = 2; i <= n; i++)
        for (int j = 0; j <= min(k, i - 2); j++) { //枚举j
            //保留第i本, 计算dp[i][j][i]
            for (int l = i - 1 - j; l < i; l++) //枚举l
                dp[i][j][i] = min(dp[i][j][i], dp[i - 1][j][l] + abs(books[i].width - books[l].width));
            //不保留第i本
            for (int l = i - j; l < i; l++) //枚举l
                if (j > 0) dp[i][j][l] = dp[i - 1][j - 1][l];
        }
    int ans = inf;
    for (int l = n - k; l <= n; l++)
        ans = min(ans, dp[n][k][l]); //统计答案
    printf("%d", ans);
    return 0;
}

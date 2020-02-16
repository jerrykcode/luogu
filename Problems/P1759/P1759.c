#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#define maxn 101
#define maxm 201
#define maxv 201

int dp_[maxm][maxv];
int m_[maxn];
int v_[maxn];
bool select_[maxn][maxm][maxv];
int result_[maxn], top_ = 0;

int main() {
    int m, v, n;
    scanf("%d %d %d", &m, &v, &n);
    int ti;
    for (int i = 1; i <= n; i++) {
        scanf("%d %d %d", m_ + i, v_ + i, &ti);
        for (int j = m; j >= m_[i]; j--) {
            for (int k = v; k >= v_[i]; k--) {
                if (dp_[j - m_[i]][k - v_[i]] + ti > dp_[j][k]) {
                    dp_[j][k] = dp_[j - m_[i]][k - v_[i]] + ti;
                    select_[i][j][k] = true;
                }
                if (i == n) break;
            }
            if (i == n) break;
        }
    }
    printf("%d\n", dp_[m][v]);
    for (int i = n; i > 0; i--)
        if (select_[i][m][v]) {
            result_[top_++] = i;
            m -= m_[i];
            v -= v_[i];
        }
    for (int i = top_ - 1; i >= 0; i--) {
        if (i != top_ - 1) putchar(' ');
        printf("%d", result_[i]);
    }
    return 0;
}

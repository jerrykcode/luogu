#include <stdio.h>
#include <stdlib.h>

#define MAXN 2001
#define MAXM 2001

unsigned char board[MAXN][MAXM];
short left[MAXN][MAXM];
short right[MAXN][MAXM];
short up[MAXN];

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%hhd", &board[i][j]);
            if (j && board[i][j - 1] ^ board[i][j])
                left[i][j] = left[i][j - 1];
            else left[i][j] = j;
        }
        for (j = m - 1; j >= 0; j--) {
            if (j + 1 < m && board[i][j + 1] ^ board[i][j])
                right[i][j] = right[i][j + 1];
            else right[i][j] = j;
        }
    }
    int l, r, w, ans0 = 0, ans1 = 0;
    for (j = 0; j < m; j++) {
        for (i = 0; i < n; i++) {
            if (i && board[i - 1][j] ^ board[i][j]) {
                l = max(l, left[i][j]);
                r = min(r, right[i][j]);
                up[i] = up[i - 1] + 1;
            }
            else {
                l = left[i][j], r = right[i][j];
                up[i] = 1;
            }
            w = r - l + 1;
            ans0 = max(ans0, min(w, up[i]) * min(w, up[i]));
            ans1 = max(ans1, w * up[i]);
        }
    }
    printf ("%d\n%d\n", ans0, ans1);
    exit(0);
}

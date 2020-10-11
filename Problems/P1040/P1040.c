#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxn 30
typedef long long ll;

int a[maxn];
ll f[maxn][maxn];
int g[maxn][maxn];

//递归输出
void print(int start, int end) {
    static bool flag = false;//记录是否输出空格
    if (start > end) return;
    if (flag) putchar(' ');
    else flag = true;
    int root = g[start][end];
    printf("%d", root + 1); //下标从0开始改为从1开始
    print(start, root - 1);
    print(root + 1, end);
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        f[i][i] = a[i];
        g[i][i] = i;
    }
    int end;
    ll left, right;
    //区间DP
    for (int len = 2; len <= n; len++)
        for (int start = 0; start <= n - len; start++) {
            end = start + len - 1;
            for (int root = start; root <= end; root++) {
                left = (start <= root - 1 ? f[start][root - 1] : 1);
                right = (root + 1 <= end ? f[root + 1][end] : 1);
                if (left * right + a[root] > f[start][end]) {
                    f[start][end] = left * right + a[root];
                    g[start][end] = root;
                }
            }
        }
    printf("%lld\n", f[0][n - 1]);
    print(0, n - 1);
    exit(0);
}

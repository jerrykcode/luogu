#include "stdio.h"
#include "stdlib.h"

double max(double a, double b) { return a > b ? a : b; }
double min(double a, double b) { return a < b ? a : b; }

double dfs(double x, double y, int n) {
    if (n == 1) {
        return max(x, y) / min(x, y);
    }
    double dx = x/n;
    double dy = y/n;
    double res = 11111.00;
    for (int i = 1; i <= n / 2; i++) {
        res = min(res, max(dfs(dx*i, y, i), dfs(x - dx*i, y, n - i)));
        res = min(res, max(dfs(x, dy*i, i), dfs(x, y - dy*i, n - i)));
    }
    return res;
}

int main() {
    double x, y;
    int n;
    scanf("%lf %lf %d", &x, &y, &n);
    printf("%.6lf\n", dfs(x, y, n));
    exit(0);
}

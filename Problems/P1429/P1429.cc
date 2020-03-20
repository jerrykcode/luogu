#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using std::sort;

#define maxn 200000
#define inf 0x7fffffff

struct Point {
    double x, y;
} points[maxn];

int a[maxn], k = 0;

bool cmp(Point& a, Point& b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
}

bool cmp_py(int a, int b) {
    return points[a].y < points[b].y;
}

double dist(Point& a, Point& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

double min(double a, double b) { return a < b ? a : b; }

double abs(double x) { return x >= 0 ? x : 0 - x; }

double merge(int left, int right) {
    if (left == right) {
        return inf;
    }
    if (left + 1 == right) {
        return dist(points[left], points[right]);
    }
    int mid = (left + right) >> 1;
    double d1 = merge(left, mid);
    double d2 = merge(mid + 1, right);
    double d = min(d1, d2);
    k = 0; 
    for (int i = left; i <= right; i++)
        if (abs(points[mid].x - points[i].x) <= d)
            a[k++] = i;
    sort(a, a + k, cmp_py);
    for (int i = 0; i < k; i++)
        for (int j = i + 1; j < k && (abs(points[a[j]].y - points[a[i]].y)) < d; j++)
            d = min(d, dist(points[a[i]], points[a[j]]));
    return d;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &points[i].x, &points[i].y);
    }
    sort(points, points + n, cmp);
    printf("%.4lf", merge(0, n - 1));
    return 0;
}

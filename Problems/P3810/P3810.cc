#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

#define MAXN 100005
#define MAXK 200005

struct node {
    int a, b, c, ans, cnt;
    inline const bool operator ==(node &n) const {
        return a == n.a && b == n.b && c == n.c;
    }
} in[MAXN], s[MAXN];

int mx, c[MAXK];
static int lowbit(int x) { return x & -x; }
static void add(int x, int y) {
    for (; x <= mx; x += lowbit(x)) c[x] += y;
}
static int sum(int x) {
    int res = 0;
    for (; x >= 1; x -= lowbit(x)) res += c[x];
    return res;
}

static bool cmp2(node x, node y) { return x.b != y.b ? x.b < y.b : x.c < y.c; }
static bool cmp1(node x, node y) { return x.a != y.a ? x.a < y.a : cmp2(x, y); }

static void cdq(int l, int r) {
    if (l == r) return;
    int mid = (l+r) >> 1;
    cdq(l, mid);
    cdq(mid + 1, r);
    sort(s + l, s + mid + 1, cmp2);
    sort(s + mid + 1, s + r + 1, cmp2);
    int j = l;
    for (int i = mid + 1; i <= r; i++) {
        while (s[j].b <= s[i].b && j <= mid) {
            add(s[j].c, s[j].cnt);
            j++;
        }
        s[i].ans += sum(s[i].c);
    }
    for (j--; j >= l; j--) add(s[j].c, -s[j].cnt);
}

int ans[MAXN];

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    mx = k;
    for (int i = 0; i < n; i++)
        scanf("%d %d %d", &in[i].a, &in[i].b, &in[i].c);
    sort(in, in + n, cmp1);
    int m = 0, cnt = 1;
    for (int i = 0; i < n; i++) {
        if (in[i] == in[i + 1])
            cnt++;
        else {
            s[m].a = in[i].a, s[m].b = in[i].b, s[m].c = in[i].c;
            s[m].cnt = cnt;
            cnt = 1;
            m++;
        }
    }
    cdq(0, m - 1);
    for (int i = 0; i < m; i++)
        ans[s[i].ans + s[i].cnt - 1] += s[i].cnt;
    for (int i = 0; i < n; i++)
        printf("%d\n", ans[i]);
    exit(0);
}

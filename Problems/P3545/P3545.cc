#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
using std::priority_queue;
using std::vector;

#define N 250000

int a[N];
bool collect[N];

struct PNode {
    int b, i;
    PNode(int b, int i) : b(b), i(i) {
    }
};

struct cmp {
    bool operator() (PNode& x, PNode& y) {
	return x.b < y.b;
    }
};

priority_queue<PNode, vector<PNode>, cmp> q;

int main() {
    int n, b;
    long long sum = 0ll;
    int ans = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
	scanf("%d", a + i);
    for (int i = 0; i < n; i++) {
	scanf("%d", &b);
	sum += a[i];
	if (sum > b) {
	    sum -= b;
	    ans++;
	    collect[i] = true;
	    q.push(PNode(b, i));
	}
	else if( (!q.empty()) && q.top().b > b) {
	    sum += q.top().b;
	    collect[q.top().i] = false;
	    q.pop();
	    sum -= b;
	    collect[i] = true;
	    q.push(PNode(b, i));
	}
    }
    printf("%d\n", ans);
    bool first = true;
    for (int i = 0; i < n; i++)
	if (collect[i]) {
	    if (first) first = false;
	    else putchar(' ');
	    printf("%d", i + 1);
	}
    exit(0);
}

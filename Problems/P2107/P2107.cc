#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

#define N 100005

class node {
    friend class Main;
    long long x;
    int t;
public:
    auto operator < (node& n) -> bool {
        return x < n.x;
    }
} nodes[N];

class cmp {
public:
    auto operator () (int& a, int& b) -> bool {
        return a < b;
    }
};

priority_queue<int, vector<int>, cmp> q;

class Main {
public:
    static void solve() {
        int n, i, ac, ans;
        long long m, time;
        scanf("%d %lld", &n, &m);
        for (i = 0; i < n; i++) {
            scanf("%lld %d", &nodes[i].x, &nodes[i].t);
        }
        sort(nodes, nodes + n);
        time = 0ll;
        ac = ans = 0;
        for (i = 0; i < n; i++) {
            time += nodes[i].x - (i ? nodes[i - 1].x : 0);
            time += nodes[i].t;
            q.push(nodes[i].t);
            ac++;
            while (time > m && !q.empty()) {
                ac--;
                time -= q.top();
                q.pop();
            }
            if (time > m) break;
            if (ac > ans) ans = ac;
        }
        priority_queue<int, vector<int>, cmp>().swap(q);
        printf("%d", ans);
    }
};

int main() {
    Main::solve();
    exit(0);
}

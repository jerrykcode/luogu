#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
#include <queue>

using namespace std;

#define DIV_ROUND_UP(X, Y) ((X+Y-1) / (Y))
#define MAXN 500000

typedef long long ll;

int a[MAXN + 1], fa[MAXN + 1], num_children[MAXN + 1], ok_num[MAXN + 1];
ll dp[MAXN + 1];
multiset<ll> s[MAXN + 1];
queue<int> q;

int main() {
    int n, t, c;
    cin >> n >> t >> c;
    a[0] = c;
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &fa[i], &a[i]);
        num_children[fa[i]]++;
    }
    for (int i = 0; i <= n; i++) {
        if (num_children[i] == 0) {
            dp[i] = a[i];
            q.push(i);
        }
        else {
            ll tmp = (ll)a[i] * (ll)num_children[i];
            ok_num[i] = (int)DIV_ROUND_UP(tmp, t);
        }
    }
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        if (dp[node] == 0) {
            auto it = s[node].begin();
            while (ok_num[node]--) {
                dp[node] += *it;
                it++;
            }
            s[node].clear();
        }
        if (node == 0) {
            break;
        }
        int father = fa[node];
        s[father].insert(dp[node]);
        if (--num_children[father] == 0) {
            q.push(father);
        }
    }
    cout << dp[0] << endl;
    exit(0);
}

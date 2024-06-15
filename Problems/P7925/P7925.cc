#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>

struct node { 
    int v; int a; 
    node(int v, int a) : v(v), a(a) {}
    bool operator <(const node& other) const {
        if (a != other.a) return a > other.a;
        return v < other.v;
    }
};
std::unique_ptr<int []> a;
std::unique_ptr<std::set<node>[]> children;
long long dfs(int v, long long s) {
    long long ans = s + a[v], child_ans;
    if (ans < 0) return -1;
    for (auto it : children[v]) {
        if ((child_ans = dfs(it.v, ans)) > 0) {
            ans += child_ans;
        }
    }
    return ans - s;
}
int main() {
    int n, s;
    std::cin >> n >> s;
    {
        std::unique_ptr<int []> p = std::make_unique<int []>(n);
        for (int i = 1; i < n; i++) scanf("%d", p.get() + i);
        a = std::make_unique<int []>(n);
        for (int i = 0; i < n; i++) scanf("%d", a.get() + i);
        children = std::make_unique<std::set<node>[]>(n);
        for (int i = 1; i < n; i++) {
            p[i]--;
            children[p[i]].insert(node(i, a[i]));
        }
    }
    long long ans = dfs(0, s);
    if (ans < 0) ans = 0;
    ans += s;
    std::cout << ans << std::endl;
    exit(0);
}

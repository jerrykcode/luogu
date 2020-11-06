#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;
#define maxn 2000
#define maxk maxn

struct AdjNode {
	int v;
	int weight;
	AdjNode(int v, int weight) : v(v), weight(weight) {}
};
vector<AdjNode> tree[maxn];

typedef long long ll;

ll dp[maxn][maxk];
int n_, m_;
int min(int a, int b) { return a < b ? a : b; }
ll max(ll a, ll b) { return a > b ? a : b; }
int dfs(int u, int parent) {
	int u_size = 1;
	for (auto it = tree[u].begin(); it != tree[u].end(); it++) {
		int v = it->v;
		if (v == parent) continue;
		ll dis = (ll)it->weight;
		int v_size = dfs(v, u);
		u_size += v_size;
		for (int j = min(u_size, m_); j>= 0; j--) {
			//j倒序

			if (j <= u_size - v_size)
				dp[u][j] += dp[v][0] + v_size * (n_ - m_ - v_size) * dis;
			for (int k = min(v_size, j); k; k--) {
				if (j - k > u_size - v_size) break;
				ll val = (k * (m_ - k) + (v_size - k) * (n_ - m_ - v_size + k)) * dis;
				dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[v][k] + val);
			}
/*
			//k正序 TLE
			for (int k = 0; k <= min(v_size, j); k++) {
				if (j - k > u_size - v_size) continue;
				ll val = (k * (m_ - k) + (v_size - k) * (n_ - m_ - v_size + k)) * dis;
				dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[v][k] + val);
			}
*/
		}
	}
	return u_size;
}

int main () {
	scanf("%d %d", &n_, &m_);
	int fr, to, dis;
	for (int i = 0; i < n_ - 1; i++) {
		scanf("%d %d %d", &fr, &to, &dis);
		fr--; to--;
		tree[fr].push_back(AdjNode(to, dis));
		tree[to].push_back(AdjNode(fr, dis));
	}
	dfs(0, -1);
	printf("%lld\n", dp[0][m_]);
	exit(0);
}

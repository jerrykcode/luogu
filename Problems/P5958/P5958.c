#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#define N 500005
#define X 10000000
struct edge {
	int to;
	int next;
} e[N];
int head[N], cnt, fa[N], size[N];
int dfs_size(int v) {
	size[v] = 1;
	for (int i = head[v]; i != -1; i = e[i].next)
		size[v] += dfs_size(e[i].to);
	return size[v];
}
bool dfs_dp(int v, double x, int *max_amount) {
	if (head[v] == -1) { if (1 > *max_amount) *max_amount = 1; return true; }
	for (int i = head[v]; i != -1; i = e[i].next)
		if (dfs_dp(e[i].to, x, max_amount) && size[e[i].to] * 1.0 / (size[v] - 1) >= x) {
			if (size[v] > *max_amount) *max_amount = size[v];
			return true;
		}
	return false;
}
int main() {
	int n, k;
	scanf("%d %d", &n, &k);
	cnt = 0;
	memset(head, 0xff, sizeof(head));
	memset(fa, 0xff, sizeof(fa));
	for (int i = 1, p; i < n; i++) {
		scanf("%d", &p); p--;
		e[cnt].to = i;
		e[cnt].next = head[p];
		head[p] = cnt++;
		fa[i] = p;
	}
	int root = -1; for (int i = 0; i < n; i++) if (fa[i] == -1) { root = i; break; }
	dfs_size(root);
	int left = 0, right = X, mid, res = X, max_amount;
	while (left <= right) {
		mid = (left + right) / 2;
		max_amount = 0;
		dfs_dp(root, mid * 1.0 / X, &max_amount);
		if (max_amount <= k) {
			res = mid;
			right = mid - 1;
		} else { left = mid + 1; }
	}
	printf("%lf", res * 1.0 / X);
	exit(0);
}

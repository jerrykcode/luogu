#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#define _vi std::vector<int>
#define _vb std::vector<bool>
#define __f(vec, val) std::fill(vec.begin(), vec.end(), val)
const int _n = 55;
int c[_n][_n];
_vi hl(_n);
_vi hr(_n);
_vi l2r(_n);
_vi r2l(_n);
_vi path(_n);
_vi dist(_n);
_vb treel(_n);
_vb treer(_n);
_vi remain(_n);
int remain_num;
int n;
int augment(int i) {
	__f(dist, 0x7fffffff);
	__f(treel, false);
	__f(treer, false);
	for (remain_num = 0; remain_num < n; remain_num++) 
		remain[remain_num] = n - remain_num - 1;
	int minVal = 0;
	int l = i, r, min_r, min_idx;
	while (1) {
		treel[l] = true;
		min_r = -1;
		for (int idx = 0; idx < remain_num; idx++) {
			r = remain[idx];
			if (minVal + c[l][r] - hl[l] - hr[r] < dist[r]) {
				dist[r] = minVal + c[l][r] - hl[l] - hr[r];
				path[r] = l;
			}
			if (min_r == -1 || dist[r] < dist[min_r]) {
				min_r = r;
				min_idx = idx;
			}
		}
		if (min_r == -1) return -1;
		if (r2l[min_r] == -1) return min_r;
		l = r2l[min_r];
		minVal = dist[min_r];
		treer[min_r] = true;
		remain[min_idx] = remain[--remain_num];
	}
	return -1;
}
bool match(int *res) {
	__f(hl, 0);
	__f(hr, 0);
	__f(l2r, -1);
	__f(r2l, -1);
	for (int i = 0; i < n; i++) if (l2r[i] == -1) {
		int j = augment(i);
		if (j == -1) return false;
		hl[i] += dist[j];
		for (int l = 0; l < n; l++) if (l != i && treel[l]) {
			hl[l] += dist[j] - dist[l2r[l]];
		}
		for (int r = 0; r < n; r++) if (treer[r]) {
			hr[r] -= dist[j] - dist[r];
		}
		for (int pre; j != -1; j = pre) {
			pre = l2r[path[j]];
			l2r[path[j]] = j;
			r2l[j] = path[j];
		}
	}
	*res = 0;
	for (int i = 0; i < n; i++)
		*res += c[i][l2r[i]];
	return true;
}
int main() {
	std::cin>>n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) 
			scanf("%d", &c[i][j]);
	// min
	int ans;
	bool success = match(&ans);
	//if (!success)  printf("error");
	printf("%d\n", ans);
	// max
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) c[i][j] *= -1;
	success = match(&ans);
	//if (!success) printf("error");
	printf("%d", -1 * ans);
	exit(0);
}
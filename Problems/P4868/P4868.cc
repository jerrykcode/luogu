#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>
using namespace std;

typedef long long LL;

int lowbit(int x) {
	return x & -x;
}

int n;

void update(LL * bit, int idx, LL k) {
	for (; idx <= n; idx += lowbit(idx)) {
		bit[idx] += k;
	}
}

LL sum(LL * bit, int idx) {
	LL result = 0;
	for (; idx > 0; idx -= lowbit(idx)) {
		result += bit[idx];
	}
	return result;
}

int main() {
	int m;
	scanf("%d %d", &n, &m);
	LL * bit_a = (LL *)malloc((n + 1) * sizeof(LL));
	LL * bit_b = (LL *)malloc((n + 1) * sizeof(LL));
	for (int i = 1; i < n + 1; i++) {
		bit_a[i] = bit_b[i] = 0;
	}
	LL *a = (LL *)malloc((n + 1) * sizeof(LL));
	for (int i = 1; i < n + 1; i++) {
		scanf("%lld", a + i);
		update(bit_a, i, a[i]);
		update(bit_b, i, a[i]*i);
	}
	//char cmd[6];
	string cmd;
	int idx;
	LL x;
	for (int i = 0; i < m; i++) {
		//scanf("%s", cmd);
		cin >> cmd;
		if (cmd[0] == 'M') {
			scanf("%d %lld", &idx, &x);
			update(bit_a, idx, x - a[idx]);
			update(bit_b, idx, (x - a[idx]) * idx);
			a[idx] = x;		
		}
		else {
			scanf("%d", &idx);			
			printf("%lld\n", sum(bit_a, idx) * (idx + 1) - sum(bit_b, idx));
		}		
	}
	free(bit_a);
	free(bit_b);
	free(a);
	return 0;
}
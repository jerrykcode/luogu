#include "stdio.h"
#include "stdlib.h"

int n;

int lowbit(int x) {
	return x & -x;
}

void update(int * bit, int x, int k) {
	for (; x <= n; x += lowbit(x)) {
		bit[x] += k;
	}
}

int sum(int * bit, int x) {
	//return sum of [1, x]
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += bit[x];
	}
	return result;
}

int * e_bit, *f_bit;
void update_ef(int x, int k) {
	update(e_bit, x, k);
	update(f_bit, x, k * x); //fi = ei * i
}

int main() {
	int m;
	scanf("%d %d", &n, &m);
	int * a = (int *)malloc((n + 1) * sizeof(int));
	e_bit = (int *)malloc((n + 1) * sizeof(int));
	f_bit = (int *)malloc((n + 1) * sizeof(int));
	for (int i = 1; i <= n; i++) {
		e_bit[i] = f_bit[i] = 0;
		scanf("%d", a+ i);
	}	
	//e1 = d1 = a1
	//f1 = e1 * 1	
	update_ef(1, a[1]);
	//e2 = d2 - d1 = (a2 - a1) - a1 = a2 - 2a1
	//f2 = e2 * 2
	update_ef(2, a[2] - 2 * a[1]);
	for (int i = 3, e; i <= n; i++) {
		//di = ai - a(i - 1)
		//ei = di - d(i - 1) = ai - 2a(i - 1) + a(i - 2)
		//fi = ei * i
		e = a[i] - 2 * a[i - 1] + a[i - 2];
		update_ef(i, e);
	}
	free(a);
	int cmd, l, r, k, d, p;
	for (int i = 0; i < m; i++) {
		scanf("%d", &cmd);
		if (cmd == 1) {
			scanf("%d %d %d %d", &l, &r, &k, &d);		
			update_ef(l, k);
			update_ef(l + 1, d - k);
			update_ef(r + 1, (l - r - 1) * d - k);
			update_ef(r + 2, (r - l) * d + k);	
		}
		else {
			scanf("%d", &p);
			printf("%d\n", sum(e_bit, p) * (p + 1) - sum(f_bit, p));
		}
	}
	free(e_bit);
	free(f_bit);
	return 0;
}
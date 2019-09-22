#include "stdio.h"
#include "stdlib.h"

typedef unsigned char bool;
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define maxcity 500000

long long a[maxcity + 1];
int bit[maxcity + 1];
bool has[maxcity + 1];

int lowbit(int x) {
	return x & -x;
}

void add(int x, int k) {
	for (; x <= maxcity; x += lowbit(x)) {
		bit[x] += k;
	}
}

int get_sum(int x) {
	int result = 0;
	for (; x > 0; x -= lowbit(x)) {
		result += bit[x];
	}
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	long long sum = 0;
	int i;
	for (i = 1; i < maxcity + 1; i++) bit[i] = 0;
	for (i = 1; i <= n; i++) {
		scanf("%lld", a + i);
		sum += a[i];
		add(i, 1);
		has[i] = TRUE;
	}
	for (; i < maxcity + 1; i++) {
		a[i] = 0;
		has[i] = FALSE;
	}
	char cmd;
	int x, y;
	for (i = 0; i < m; i++) {
		scanf(" %c", &cmd);
		switch (cmd) {
		case 'C': {
			scanf("%d %d", &x, &y);
			sum -= y;
			a[x] -= y;
			break;
		}
		case 'I': {
			scanf("%d %d", &x, &y);
			sum += y - a[x];
			a[x] = y;
			if (!has[x]) {
				has[x] = TRUE;
				add(x, 1);
			}
			break;
		}
		case 'D': {
			scanf("%d", &x);
			int left = 1, right = maxcity, mid, s;
			while (left <= right) {
				mid = (left + right) / 2;
				s = get_sum(mid);
				if (s == x) {
					if (has[mid]) {
						sum -= a[mid];
						a[mid] = 0;
						has[mid] = FALSE;
						add(mid, -1);
						break;
					}
					else {
						right = mid - 1;
					}
				}
				else if (s < x) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			break;
		}
		case 'Q': {
			printf("%lld\n", sum);
			break;
		}
		default:
			break;
		}
	}
	return 0;
}
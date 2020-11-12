#include <stdio.h>
#include <stdlib.h>

#define maxn 4000
int dp[maxn][maxn];

typedef struct Node {
	short date;
	int value;
} Node;
Node queue[maxn];
int top, last;

void Init() {
	top = 0;
	last = -1;
}

int Top() {
	return queue[top].value;
}

void Push(short date, int value) {
	while (top <= last && value >= queue[last].value)
		last--;
	queue[++last].date = date;
	queue[last].value = value;
}

void Expire(short date) {
	if (queue[top].date < date) top++;
}

int main() {
	int n, m, k, t, x, y, v;
	scanf("%d %d %d %d", &n, &m, &k, &t);
	for (int i = 0; i < k; i++) {
		scanf("%d %d %d", &x, &y, &v);
		dp[--x][--y] = v;
	}
	for (int i = n - 2; i >= 0; i--)
		for (int j = 0; j < m; j++) {
			if (j == 0) {
				Init();
				for (int s = 0; s <= t; s++)
					Push(s, dp[i + 1][s]);
			}
			else {
				if (j + t < m) Push(j + t, dp[i + 1][j + t]);
				Expire(j - t);
			}
			dp[i][j] += Top();
		}
	int ans = 0;
	for (int j = 0; j < m; j++)
		if (dp[0][j] > ans) ans = dp[0][j];
	printf("%d\n", ans);
	exit(0);
}

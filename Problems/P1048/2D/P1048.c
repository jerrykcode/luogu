#include "stdio.h"
#include "stdlib.h"
#include "memory.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

int main() {
	int t, m;
	scanf("%d %d", &t, &m);
	int *time = (int *)malloc((m + 1) * sizeof(int));
	int *value = (int *)malloc((m + 1) * sizeof(int));
	for (int i = 1; i < m + 1; i++) {
		scanf("%d %d", time + i, value + i);
	}
	int **dp = (int **)malloc((m + 1) * sizeof(int *));
	for (int i = 0; i < m + 1; i++) {
		dp[i] = (int *)malloc((t + 1) * sizeof(int));
		dp[i][0] = 0;
	}
	memset(dp[0], 0, (t + 1) * sizeof(int));
	for (int i = 1; i < m + 1; i++)
		for (int j = 1; j <= t; j++) {
			if (j >= time[i]) {
				dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - time[i]] + value[i]);
			}
			else dp[i][j] = dp[i - 1][j];
		}
	free(time);
	free(value);
	printf("%d", dp[m][t]);
	for (int i = 0; i < m + 1; i++)
		free(dp[i]);
	free(dp);
	return 0;
}
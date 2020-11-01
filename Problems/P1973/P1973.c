#include <stdio.h>
#include <stdlib.h>

#define maxn 200
#define maxt maxn << 1

int start[maxn], end[maxn];
int p[maxt];
short q[maxt];
typedef unsigned char byte;
byte inter[maxt][maxt];
byte pre[maxt][maxn], suf[maxt][maxn];
byte res[maxt][maxt];

int compare(const void *a, const void *b) { return *(int*)a - *(int*)b; }

byte max(byte a, byte b) { return a > b ? a : b; }
byte min(byte a, byte b) { return a < b ? a : b; }

int main() {
	int n;
    scanf("%d", &n);
	for (int i = 0; i < n; i++) {
        scanf("%d %d", &start[i], &end[i]);
        end[i] += start[i];
        p[i << 1] = start[i];
        p[i << 1 | 1] = end[i];
    }
    qsort(p, n << 1, sizeof(int), compare);
    int t = 0;
    for (int i = 0; i < n << 1; i++)
        q[i] = (i && p[i] == p[i - 1]) ? q[i - 1] : t++;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n << 1; j++) {
            if (start[i] == p[j]) start[i] = q[j];
            if (end[i] == p[j]) end[i] = q[j];
        }
    for (int i = 0; i < t; i++)
        for (int j = i + 1; j < t; j++)
            for (int k = 0; k < n; k++)
                if (start[k] >= i && end[k] <= j) inter[i][j]++;
    for (int i = 1; i < t; i++) {
        pre[i][0] = inter[0][i];
        pre[i][inter[0][i]] = 0;
        for (int j = 1; j < inter[0][i]; j++)
            for (int k = 1; k < i; k++) {
                if (inter[0][k] >= j) pre[i][j] = max(pre[i][j], pre[k][j] + inter[k][i]);
                pre[i][j] = max(pre[i][j], pre[k][(j > inter[k][i] ? j - inter[k][i] : 0)]);
            }
    }
    byte ans0 = 0;
    for (int i = 1; i < inter[0][t - 1]; i++)
        ans0 = max(ans0, min(i, pre[t - 1][i]));
    printf("%u\n", ans0);
    for (int i = t - 2; i >= 0; i--) {
        suf[i][0] = inter[i][t - 1];
        suf[i][inter[i][t - 1]] = 0;
        for (int j = 1; j < inter[i][t - 1]; j++)
            for (int k = t - 2; k > i; k--) {
                if (inter[k][t - 1] >= j) suf[i][j] = max(suf[i][j], inter[i][k] + suf[k][j]);
                suf[i][j] = max(suf[i][j], suf[k][(j > inter[i][k] ? j - inter[i][k] : 0)]);
            }
    }
    for (int i = 0; i < t; i++)
        for (int j = i + 1; j < t; j++) {
            int y = inter[j][t - 1];
            for (int x = 0; x <= inter[0][i]; x++) {
                byte cur = min(x + inter[i][j] + y, pre[i][x] + suf[j][y]);
                while (y) {
                    byte next = min(x + inter[i][j] + y - 1, pre[i][x] + suf[j][y - 1]);
                    if (next < cur) break;
                    else {
                        cur = next;
                        y--;
                    }
                }
                res[i][j] = max(res[i][j], cur);
            }
        }
    for (int i = 0; i < n; i++) {
        byte ans1 = 0;
        for (int j = start[i]; j >= 0; j--)
            for (int k = end[i]; k < t; k++)
                if (res[j][k] > ans1) ans1 = res[j][k];
        printf("%u\n", ans1);
    }
	exit(0);
}

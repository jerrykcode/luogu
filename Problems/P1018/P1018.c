#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxn 40
#define maxk 7

typedef char *big_int;

static big_int dp[maxn][maxn][maxk];
static big_int num;

static big_int sub(int l, int r) {
    size_t len = r - l + 1;
    char *s = (char *)malloc((len + 1)*sizeof(char));
    strncpy(s, num + l, len);
    s[len] = '\0';
    return s;
}

static big_int mul(big_int a, big_int b) {
    if (a == NULL || b == NULL) {
        printf("这个分支不会被执行~~~~");
        char *res = (char *)malloc(2 * sizeof(char));
        res[0] = '0';
        res[1] = '\0';
        return res;
    }
    size_t a_len = strlen(a);
    size_t b_len = strlen(b);
    unsigned char *val = (unsigned char *)malloc((a_len + b_len) * sizeof(unsigned char));
    memset(val, 0, (a_len + b_len) * sizeof(unsigned char));
    for (int i = a_len - 1; i >= 0; i--)
        for (int j = b_len - 1; j >=0 ;j--) {
            val[i + j + 1] += (a[i] - '0')*(b[j] - '0');
            val[i + j] += val[i + j + 1] / 10;
            val[i + j + 1] %= 10;
        }
    int index = 0;
    while (index < a_len + b_len && val[index] == 0)
        index++;
    if (index == a_len + b_len) index--;
    char *res = (char *)malloc((a_len + b_len - index + 1)*sizeof(char));
    for (int i = index; i < a_len + b_len; i++)
        res[i - index] = val[i] + '0';
    res[a_len + b_len - index] = '\0';
    free(val);
    return res;
}

static big_int max(char *a, char *b) {
    if (a == NULL)
        return b;
    if (b == NULL)
        return a;
    size_t a_len = strlen(a);
    size_t b_len = strlen(b);
    if (a_len > b_len) 
        return a;
    else if (a_len < b_len)
        return b;
    
    for (int i = 0; i < a_len; i++) {
        if (a[i] > b[i]) 
            return a;
        else if (a[i] < b[i])
            return b;
    }
    return a;
}

static void print(big_int a) {
    printf("%s\n", a);
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    k++;
    num = (char *)malloc(n*sizeof(char));
    scanf("%s", num);
    int len, l, r, t, mt, m, t1, mt1, mt2;
    for (len = 1; len <= n; len++)
        for (l = 0; l < n - len + 1; l++) {
            r = l + len - 1;
            dp[l][r][1] = sub(l, r);            
            mt = len < k ? len : k;
            for (t = 2; t <= mt; t++)
                for (m = l; m < r; m++) {
                    mt1 = (m - l + 1) < t - 1 ? (m - l + 1) : t - 1;
                    mt2 = (r - m) < t - 1 ? (r - m) : t - 1;
                    for (t1 = 1; t1 <= mt1; t1++) {
                        if (t - t1 > mt2) continue; 
                        big_int a = mul(dp[l][m][t1], dp[m + 1][r][t - t1]);
                        big_int b = dp[l][r][t];
                        dp[l][r][t] = max(a, b);
                        if (dp[l][r][t] == a) { 
                            if (b != NULL)
                                free(b);
                        }
                        else {
                            free(a);
                        }
                    }
                }
        }
    free(num);
    print(dp[0][n - 1][k]);
    for (len = 1; len <= n; len++)
        for (l = 0; l < n - len + 1; l++) {
            r = l + len - 1;
            mt = len < k ? len : k;
            for (int t = 1; t < mt; t++)
                free(dp[l][r][t]);
        }
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>

#define maxn 80
#define maxm 80
int m;

//typedef long long ll;
typedef __uint128_t ll;
ll f[maxm + 1][maxm + 1][2];
int row[maxm + 1];

static ll pow(int i) {
    return (ll)1 << i;
}

static ll max(ll a, ll b) {
    return a > b ? a : b;
}

static ll dp() {
    f[1][1][0] = row[1] * 2;
    f[1][m][1] = row[m] * 2;
    for (int i = 2; i <= m; i++) {
        f[i][1][0] = row[1]*pow(i) + f[i - 1][m - i + 2][1]; //行首为1，上一次取数只能从行尾取
        for (int j = 2; j < i; j++)
            f[i][j][0] = row[j]*pow(i) + max(f[i - 1][j - 1][0], f[i - 1][m - i + j + 1][1]);
        f[i][i][0] = row[i]*pow(i) + f[i - 1][i - 1][0]; //行首为i，说明之前每次取数均从行首取

        f[i][m][1] = row[m]*pow(i) + f[i - 1][i - 1][0]; //行尾为m, 上一次取数只能从行首取
        for (int j = m - 1; j > m - i + 1; j--)
            f[i][j][1] = row[j]*pow(i) + max(f[i - 1][j + 1][1], f[i - 1][i + j - m - 1][0]);
        f[i][m - i + 1][1] = row[m - i + 1]*pow(i) + f[i - 1][m - i + 2][1]; //行尾为m - i + 1，说明之前每次均从行尾取
    }
    ll res = 0;
    for (int i = 1; i <= m; i++) {
        res = max(res, f[m][i][0]);
        res = max(res, f[m][i][1]);
    }
//    printf("%lld\n", res);
    return res;
}

static char *itoa(ll i) {
    size_t len = 0;
    ll j = i;
    while (j) {
        j /= 10;
        len++;
    }
    char *str = malloc((len + 1) * sizeof(char));
    str[len] = '\0';
    while (i) {
        str[--len] = i % 10 + '0';
        i /= 10;
    }
    return str;
}

int main() {
    int n;
    scanf("%d %d", &n, &m);
    ll res = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= m; j++)
            scanf("%d", row + j);
        res += dp();
    }
    if (res) {
        char *strres = itoa(res);
        printf("%s\n", strres);
        free(strres);
    }
    else printf("0");
    exit(0);
}


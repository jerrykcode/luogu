#include <stdio.h>
#include <stdlib.h>

#define maxs 500005

int dp[maxs][3][2];
#define r 0
#define g 1
#define b 2
#define maxv 0
#define minv 1

char tree[maxs];
static size_t index = 0;

static size_t tni = 0; //tree node index

int max(int x, int y) {
    return x > y ? x : y;
}

int min(int x, int y) {
    return x < y ? x : y;
}

int max3(int x, int y, int z) {
    return max(max(x, y), z);
}

int min3(int x, int y, int z) {
    return min(min(x, y), z);
}

size_t DP() {
    size_t this_tni = tni++; //this tni
    char num_subtrees = tree[index++];
    if (num_subtrees == '0') {
        dp[this_tni][r][maxv] = dp[this_tni][r][minv] = 0;
        dp[this_tni][g][maxv] = dp[this_tni][g][minv] = 1;
        dp[this_tni][b][maxv] = dp[this_tni][b][minv] = 0;
        goto END;
    }
    size_t left_tni = DP();
    if (num_subtrees == '1') {
        //r
        dp[this_tni][r][maxv] = max(dp[left_tni][g][maxv], dp[left_tni][b][maxv]);
        dp[this_tni][r][minv] = min(dp[left_tni][g][minv], dp[left_tni][b][minv]);
        //g
        dp[this_tni][g][maxv] = 1 + max(dp[left_tni][r][maxv], dp[left_tni][b][maxv]);
        dp[this_tni][g][minv] = 1 + min(dp[left_tni][r][minv], dp[left_tni][b][minv]);
        //b
        dp[this_tni][b][maxv] = max(dp[left_tni][r][maxv], dp[left_tni][g][maxv]);
        dp[this_tni][b][minv] = min(dp[left_tni][r][minv], dp[left_tni][g][minv]);
        goto END;
    }
    size_t right_tni = DP();
    //r
    dp[this_tni][r][maxv] = max(dp[left_tni][g][maxv] + dp[right_tni][b][maxv], dp[left_tni][b][maxv] + dp[right_tni][g][maxv]);
    dp[this_tni][r][minv] = min(dp[left_tni][g][minv] + dp[right_tni][b][minv], dp[left_tni][b][minv] + dp[right_tni][g][minv]);
    //g
    dp[this_tni][g][maxv] = 1 + max(dp[left_tni][r][maxv] + dp[right_tni][b][maxv], dp[left_tni][b][maxv] + dp[right_tni][r][maxv]);
    dp[this_tni][g][minv] = 1 + min(dp[left_tni][r][minv] + dp[right_tni][b][minv], dp[left_tni][b][minv] + dp[right_tni][r][minv]);
    //b
    dp[this_tni][b][maxv] = max(dp[left_tni][r][maxv] + dp[right_tni][g][maxv], dp[left_tni][g][maxv] + dp[right_tni][r][maxv]);
    dp[this_tni][b][minv] = min(dp[left_tni][r][minv] + dp[right_tni][g][minv], dp[left_tni][g][minv] + dp[right_tni][r][minv]);
END:
    return this_tni;
}

int main() {
    scanf("%s", tree);
    size_t root_tni = DP();
    printf("%d %d\n", 
            max3(dp[root_tni][r][maxv], dp[root_tni][g][maxv], dp[root_tni][b][maxv]), 
            min3(dp[root_tni][r][minv], dp[root_tni][g][minv], dp[root_tni][b][minv])
          );
    exit(0);
}

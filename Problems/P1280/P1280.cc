#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
using namespace std;

#define N 10004
#define K 10004

struct task {
    int start, end;
    task(int start, int end) : start(start), end(end) {}
};

vector<task> tasks_start_at[N];

int dp[N];

int main() {
    int n, k, i, p, t, tmp, tmp2;
    cin >> n >> k;
    for (i = 0; i < k; i++) {
        scanf("%d %d", &p, &t);
        p--;
        tasks_start_at[p].push_back(task(p, p + t - 1));
    }
    i = n - 1;  // last
    if (tasks_start_at[i].size() == 0) {
        dp[i] = 1;
        tmp = -1;
        tmp2 = 1;
    } else {
        dp[i] = 0;
        tmp = 0;
        tmp2 = 0;
    }
    for (i = n - 2; i >= 0; i--) {
        if (tasks_start_at[i].size() > 0) {
            tmp2 = 0;
            tmp = 0;
            for (auto it = tasks_start_at[i].begin(); it != tasks_start_at[i].end(); it++)
                if (dp[it->end + 1] > tmp) tmp = dp[it->end + 1];
        } else {
            tmp2++;
        }
        if (tmp == -1) {
            dp[i] = tmp2;
        } else {
            dp[i] = tmp + tmp2;
        }
    }
    printf("%d", dp[0]);
    exit(0);
}

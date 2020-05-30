#include <stdio.h>
#include <stdlib.h>

#define maxk 500
#define maxm 500

#define OUT_INDEX(idx) (idx + 1)

int arr[maxm];

int cut(int m, int maxsum) {
    int num = 0, sum = 0;
    for (int i = m - 1; i >= 0; i--) {
        if (sum + arr[i] > maxsum) {
            sum = 0;
            num++;
        }
        sum += arr[i];
    }
    num++;
    return num;
}

void print(int m, int maxsum) {
    int sum = 0;
    for (int i = m - 1; i >= 0; i--) {
        if (arr[i] + sum > maxsum) {
            print(i + 1, maxsum);
            printf("%d %d\n", OUT_INDEX(i + 1), OUT_INDEX(m - 1));
            return;
        }
        sum += arr[i];
    }
    printf("%d %d\n", OUT_INDEX(0), OUT_INDEX(m - 1));
}

int main() {
    int m, k;
    scanf("%d %d", &m, &k);
    int maxa = 0, arrsum = 0;
    for (int i = 0; i < m; i++) {
        scanf("%d", &arr[i]);
        if (arr[i] > maxa) maxa = arr[i];
        arrsum += arr[i];
    }
    int left = maxa, right = arrsum;
    int result = right;
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (cut(m, mid) <= k) {
            result = mid;
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    print(m, result);
    return 0;
}

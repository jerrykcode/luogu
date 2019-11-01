#include "stdio.h"
#include "stdlib.h"
typedef long long LL;
int main() {
    int n;
    scanf("%d", &n);
    LL  d, pre, a;
    LL positive = 0, negative = 0;
    int i;
    for (i = 0; i < n; i++) {
        scanf("%lld", &a);
        //求差分
        //这里d没有使用数组，第i次循环的d表示d[i]
        if (i) d = a - pre;
        else d = a;
        pre = a;
        if (i) {
        //计算除d[0]外正数之和与负数绝对值之和
            d > 0 ? (positive += d) : (negative -= d);
        }
    }
    LL max = positive > negative ? positive : negative;
    LL min = positive > negative ? negative : positive;
    //最少max次操作，max - min次单独增加/减少操作，d[0]有max - min + 1种不同的值
    printf("%lld\n%lld", max, max - min + 1);
    return 0;
}
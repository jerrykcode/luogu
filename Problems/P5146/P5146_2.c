#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    scanf("%d", &n);
    int a, min_a;
    long long sub, max_sub = 0x8000000000000000;
    scanf("%d", &a); // first element
    min_a = a;
    for (int i = 1; i < n; i++) { //n - 1 elements
        scanf("%d", &a);
        sub = (long long)a - (long long)min_a;
        if (sub > max_sub) max_sub = sub;
        if (a < min_a) min_a = a;
    }
    printf("%lld\n", max_sub);
    exit(0);
}

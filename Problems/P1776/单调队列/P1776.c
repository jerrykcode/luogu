#include <stdio.h>
#include <stdlib.h>

#define maxw 40000

typedef struct Node {
    int index;
    int value;
} Node;
Node queue[maxw];
int head, tail;

void Init() {
    head = 0;
    tail = -1;
}

Node Top() {
    return queue[head];
}

void Push(int index, int value) {
    while (head <= tail && queue[tail].value <= value)
        tail--;
    tail++;
    queue[tail].index = index;
    queue[tail].value = value;
}

void Check(int index, int timeout) {
    if (queue[head].index < index - timeout)
        head++;
}

int dp[maxw + 1];

int main() {
    int n, w;
    scanf("%d %d", &n, &w);
    int vi, wi, mi;
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &vi, &wi, &mi);
        for (int j = 0; j < wi; j++) {
            Init();
            for (int k = 0; k <= (w - j) / wi; k++) {
                Push(k, dp[k * wi + j] - k * vi);
                Check(k, mi);
                dp[k * wi + j] = Top().value + k * vi;
            }
        }
    }
    printf("%d\n", dp[w]);
    exit(0);
}

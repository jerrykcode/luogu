#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxn 1000006
#define is_left(c) (c == '(' || c == '[')
#define is_right(c) (c == ')' || c == ']')

static char Pair(char c) {
    if (c == ')') return '(';
    else return '[';
}

char *str;
int stack[maxn];
char ans[maxn];
static int top = -1;

static int Empty() {
    return top == -1;
}

static int Top() {
    return Empty() ? -1 : stack[top];
}

static void Pop() {
    /*if (!Empty())*/ top--;
}

static void Push(int i) {
    stack[++top] = i;
}

static void Clear() {
    top = -1;
}

int main() {
    int len;
    getline(&str, &len, stdin);
    int start = 0, ans_start = 0, ans_len = 0;
    char c;
    for (int i = 0; i < len; i++) {
        c = str[i];
        if (c == '\n' || c == '\0') break;
        if (is_left(c)) {
            Push(i);
        }
        else {
            if (!Empty() && str[Top()] == Pair(c)) {
                Pop();
                if (!Empty()) {
                    if (ans_len < i - Top()) {
                        ans_len = i - Top();
                        ans_start = Top() + 1;
                    }
                }
                else {
                    if (ans_len < i - start + 1) {
                        ans_len = i - start + 1;
                        ans_start = start;
                    }
                }
            }
            else {
               Clear();
               start = i + 1;
            }
        }
    }
    strncpy(ans, &str[ans_start], ans_len);
    printf("%s", ans);
    exit(0);
}

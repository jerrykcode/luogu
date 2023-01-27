#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 10004

typedef struct vector {
    int size;
    int cap;
    int *arr;
} vector;

static inline vector *vcreate() {
    vector *v = (vector *)malloc(sizeof(*v));
    v->size = 0;
    v->cap = 10;
    v->arr = (int *)malloc(v->cap*sizeof(int));
    return v;
}

static inline void vfree(vector *v) {
    if (v == NULL) return;
    if (v->arr) free(v->arr);
    free(v);
}

static inline void vadd(vector *v, int ele) {
    if (v == NULL) return;
    if (++v->size > v->cap) {
        v->cap *= 2;
        v->arr = realloc(v->arr, v->cap*sizeof(int));
    }
    v->arr[v->size - 1] = ele;
}

static inline int vsize(vector *v) { return v->size; }

static inline int vget(vector *v, int i) { return v->arr[i]; }

static inline void vpush(vector *v, int ele) { vadd(v, ele); }

static inline int vpop(vector *v) { return v->arr[--v->size]; }

vector *graph[N];
int dfn[N], low[N];
bool instack[N];
int count, ans;
vector *stack;

static inline int min(int a, int b) { return a < b ? a : b; }

void dfs(int v) {
    if (dfn[v]) return;
    dfn[v] = low[v] = count++;
    vpush(stack, v);
    instack[v] = true;
    int w;
    for (int i = 0; i < vsize(graph[v]); i++) {
        w = vget(graph[v], i);
        if (!dfn[w]) {
            dfs(w);
            low[v] = min(low[v], low[w]);
        } else if (instack[w]) {
            low[v] = min(low[v], dfn[w]);
        }
    }
    int csize = 0;
    if (dfn[v] == low[v]) {
        while ((w = vpop(stack)) != v) {
            csize++;
            instack[w] = false;
        }
        csize++;
        instack[v] = false;
        if (csize > 1) ans++;
    }
}

int main() {
    int n, m, i, v, w;
    scanf("%d %d", &n, &m);
    for (i = 0; i < n; i++) graph[i] = vcreate();
    for (i = 0; i < m; i++) {
        scanf("%d %d", &v, &w);
        v--, w--;
        vadd(graph[v], w);
    }
    stack = vcreate();
    ans = 0;
    for (i = 0; i < n; i++) {
        count = 0;
        dfs(i);
    }
    printf("%d\n", ans);
    for (i = 0; i < n; i++)
        vfree(graph[i]);
    vfree(stack);
    exit(0);
}

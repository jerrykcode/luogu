#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_st {
    int l, r; // range
    int max_len;
    int lmax;
    int rmax;
    int lazy;
    struct node_st *left, *right;
} *Tree;

#define _FILL 0
#define _EMPTY 1
#define _NO 2

#define _len(t) (t->r - t->l + 1)

#define _max(a, b) ((a) > (b) ? (a) : (b))
#define _max3(a, b, c) _max(a, _max(b, c))

Tree build(int l, int r) {
    Tree t = (Tree)malloc(sizeof(struct node_st));
    t->l = l;
    t->r = r;
    t->max_len = t->lmax = t->rmax = r - l + 1;
    int mid = l + r >> 1;
    if (l == r) t->left = t->right = NULL;
    else {
        t->left = build(l, mid);
        t->right = build(mid + 1, r);
    }
    t->lazy = _NO;
    return t;
}

// Returns True if the range[l, r] contains the tree
static bool contain(int l, int r, Tree t) {
    return l <= t->l && t->r <= r;
}

// Returns True if the range[l, r] cross the tree
static bool cross(int l, int r, Tree t) {
    return r >= t->l && l <= t->r;
}

static void tag_down(Tree t) {
    if (t->lazy == _NO) return;
    if (t->left) {
        t->left->max_len = t->left->lmax = t->left->rmax = t->lazy * _len(t->left);
        t->left->lazy = t->lazy;
    }
    if (t->right) {
        t->right->max_len = t->right->lmax = t->right->rmax = t->lazy * _len(t->right);
        t->right->lazy = t->lazy;
    }
    t->lazy = _NO;
}

static void push_up(Tree t) {
    t->max_len = _max3(t->left->max_len, t->right->max_len, t->left->rmax + t->right->lmax);
    t->lmax = t->left->max_len == _len(t->left) ? t->left->max_len + t->right->lmax : t->left->lmax;
    t->rmax = t->right->max_len == _len(t->right) ? t->right->max_len + t->left->rmax : t->right->rmax;
}

void update(Tree t, int l, int r, int k) {
    if (contain(l, r, t)) {
        t->max_len = t->lmax = t->rmax = k * _len(t);
        t->lazy = k;
        return;
    }
    tag_down(t);
    if (cross(l, r, t->left)) update(t->left, l, r, k);
    if (cross(l, r, t->right)) update(t->right, l, r, k);
    push_up(t);
}

int query(Tree t, int k) {
    if (!t->left && !t->right)
        return t->l;
    tag_down(t);
    if (t->left->max_len >= k) return query(t->left, k);
    else if (t->left->rmax + t->right->lmax >= k) {
        return t->left->r - t->left->rmax + 1;
    }
    else return query(t->right, k);
}

void clear(Tree t) {
    if (t->left) clear(t->left);
    if (t->right) clear(t->right);
    free(t);
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m); getchar();
    Tree t = build(0, n - 1);
    char cmd;
    int a, b, ans = 0;
    for (int i = 0; i < m; i++) {
        scanf("%c", &cmd); getchar();
        if (cmd == 'A') {
            scanf("%d", &a); getchar();
            if (t->max_len < a) ans++;
            else {
                b = query(t, a);
                update(t, b, b + a - 1, _FILL);
            }
        }
        else { // cmd == 'L'
            scanf("%d %d", &a, &b); getchar();
            a--; b--;
            update(t, a, b, _EMPTY);
        }
    }
    clear(t);
    printf("%d", ans);
    exit(0);
}

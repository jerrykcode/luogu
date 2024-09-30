#include "stdio.h"
#include "stdlib.h"
#define N 100005
int debug_mode;
#define assert(condition) if (debug_mode && !(condition)) printf("ERROR\n"), exit(-1);
typedef unsigned int ptr_t;
#define nullptr 0
struct {
	ptr_t fa;
	ptr_t ch[2];
	int val;
	int sz;
	int lazy;
} mem[N];
#define ptr(p) (mem[p])
#define get(p, field) (ptr(p).field)
#define set(p, field, val) if (p != nullptr) ptr(p).field = val
ptr_t mem_ptr;
static inline ptr_t update_sz(ptr_t p) {
	set(p, sz, 1 + (get(p, ch[0]) != nullptr ? get(get(p, ch[0]), sz) : 0) 
					+ (get(p, ch[1]) != nullptr ? get(get(p, ch[1]), sz) : 0));
	return p;
}
ptr_t rt;
static inline int which_ch(ptr_t p) {
	return get(get(p, fa), ch[1]) == p;
}
static inline void rotate(ptr_t p) {
	if (p == nullptr) return;
	ptr_t fa = get(p, fa);
	if (fa == nullptr) return;
	int p_which_ch = which_ch(p);
	ptr_t ch = get(p, ch[p_which_ch^1]);
	ptr_t g = get(fa, fa);
	int fa_which_ch = which_ch(fa);
	set(fa, ch[p_which_ch], ch);
	set(ch, fa, fa);
	set(p, ch[p_which_ch^1], fa);
	set(fa, fa, p);
	update_sz(fa);
	update_sz(p);
	set(p, fa, g);
	if (g == nullptr) rt = p;
	else set(g, ch[fa_which_ch], p);
}
static inline void splay(ptr_t p, ptr_t goal) {
	for (ptr_t fa, g; (fa = get(p, fa)) != goal; rotate(p))
		if ((g = get(fa, fa)) != goal) rotate(which_ch(p) ^ which_ch(fa) ? p : fa);
}
static inline void pushdown(ptr_t p) {
	if (get(p, lazy)) {
		set(p, ch[0], get(p, ch[0]) + get(p, ch[1]));
		set(p, ch[1], get(p, ch[0]) - get(p, ch[1]));
		set(p, ch[0], get(p, ch[0]) - get(p, ch[1]));
		set(p, lazy, 0);
		for (int i = 0; i < 2; i++) set(get(p, ch[i]), lazy, 1 ^ get(get(p, ch[i]), lazy));
	}
}
static inline ptr_t kth(int k) {
	ptr_t cur = rt;
	while (cur != nullptr) {
		pushdown(cur);
		int left_ch_sz = get(cur, ch[0]) != nullptr ? get(get(cur, ch[0]), sz) : 0;
		if (k < left_ch_sz + 1) cur = get(cur, ch[0]);
		else if (k == left_ch_sz + 1) return cur;
		else {
			k -= left_ch_sz + 1;
			cur = get(cur, ch[1]);
		}
	}
	return nullptr;
}
static inline void reverse(int l, int r) {
	ptr_t lp = kth(l);
	splay(lp, 0);
	assert(lp==rt)
	ptr_t rp = kth(r + 2);
	splay(rp, lp);
	assert(get(rp, fa)==lp)
	set(get(rp, ch[0]), lazy, 1 ^ get(get(rp, ch[0]), lazy));
}
static inline ptr_t create_node(ptr_t fa, ptr_t ch0, ptr_t ch1, int val, int sz) {
	mem_ptr++;
	set(mem_ptr, fa, fa);
	set(mem_ptr, ch[0], ch0);
	set(mem_ptr, ch[1], ch1);
	set(mem_ptr, val, val);
	set(mem_ptr, sz, sz);
	set(mem_ptr, lazy, 0);
	return mem_ptr;
}
int a[N];
static inline ptr_t build(int l, int r, ptr_t fa) {
	if (l > r) return nullptr;
	int mid = (l + r) >> 1;
	ptr_t p = create_node(fa, nullptr, nullptr, a[mid], 1);
	set(p, ch[0], build(l, mid - 1, p));
	set(p, ch[1], build(mid + 1, r, p));
	return update_sz(p);
}
int dfs_cnt;
static inline void inorder_dfs(ptr_t p) {
	if (p == nullptr) return;
	pushdown(p);
	inorder_dfs(get(p, ch[0]));
	a[dfs_cnt++] = get(p, val);
	inorder_dfs(get(p, ch[1]));
}
#define print_ans {dfs_cnt = 0;\
	inorder_dfs(rt);\
	for (int i = 1; i < n; i++)\
		printf("%d ", a[i]);\
	printf("%d", a[n]);}
int main() {
	debug_mode = 0;
	int n, m, l, r;
	scanf("%d %d", &n, &m);
	a[0] = 520;
	for (int i = 1; i <= n; i++) {a[i] = i;  }
	a[n + 1] = 1314;
	rt = build(0, n + 1, nullptr);
	while (m--) {
		scanf("%d %d", &l, &r);
		reverse(l, r);
		if (debug_mode) {print_ans;printf("\n");}
	}
	print_ans
	exit(0);
}

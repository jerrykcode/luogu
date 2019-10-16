#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

typedef long long LL;

LL p;

typedef struct TNode {
	int start, end;
	LL sum;
	LL mul_tag, add_tag;
	struct TNode *left, *right;
} *Tree;

Tree buildTree(LL * arr, int start, int end) {
	if (start > end) return NULL;
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->start = start;
	t->end = end;
	t->mul_tag = 1;
	t->add_tag = 0;
	if (start == end) {
		t->sum = arr[start];
		t->left = t->right = NULL;
	}
	else {
		int mid = (start + end) >> 1;
		t->left = buildTree(arr, start, mid);
		t->right = buildTree(arr, mid + 1, end);
		t->sum = t->left->sum + t->right->sum;
	}
	t->sum %= p;
	return t;
}

bool containsTree(int start, int end, Tree t) {
	//判断[start, end]是否包含t的范围
	return start <= t->start && t->end <= end;
}

bool crossTree(int start, int end, Tree t) {
	//判断[start, end]是否与t的范围相交
	return start <= t->end && t->start <= end;
}

int len(Tree t) {
	//返回t的范围的长度
	return t->end - t->start + 1;
}

void mulUpdateTags(Tree t, LL mul) {
	//在t的范围乘以mul之后，更新t的mul_tag和add_tag标记
	t->mul_tag *= mul;
	t->mul_tag %= p;
	t->add_tag *= mul;
	t->add_tag %= p;
}

void addUpdateTags(Tree t, LL add) {
	//在t的范围加上add之后，更新t的add_tag标记
	t->add_tag += add;
	t->add_tag %= p;
}

void tagDown(Tree t) {
	//向下传递标记
	if (t == NULL || (t->mul_tag == 1 && t->add_tag == 0)) return;
	LL t_mul_tag = t->mul_tag, t_add_tag = t->add_tag;
	Tree t_left = t->left, t_right = t->right;
	if (t_left) {
		t_left->sum *= t_mul_tag;
		t_left->sum += t_add_tag * len(t_left);
		t_left->sum %= p;
		mulUpdateTags(t_left, t_mul_tag);
		addUpdateTags(t_left, t_add_tag);
	}
	if (t_right) {
		t_right->sum *= t_mul_tag;
		t_right->sum += t_add_tag * len(t_right);
		t_right->sum %= p;
		mulUpdateTags(t_right, t_mul_tag);
		addUpdateTags(t_right, t_add_tag);
	}
	t->mul_tag = 1;
	t->add_tag = 0;
}

LL query(int start, int end, Tree t) {
	if (t == NULL) return 0;
	if (containsTree(start, end, t)) {
		return t->sum;
	}
	tagDown(t);
	LL result = 0;
	if (t->left && crossTree(start, end, t->left)) {
		result += query(start, end, t->left);
	}
	if (t->right && crossTree(start, end, t->right)) {
		result += query(start, end, t->right);
	}
	return result % p;
}

void mulUpdate(int start, int end, Tree t, LL mul) {
	if (t == NULL) return;
	if (containsTree(start, end, t)) {
		t->sum *= mul;
		t->sum %= p;
		mulUpdateTags(t, mul); //更新标记
		return;
	}
	tagDown(t);
	if (t->left && crossTree(start, end, t->left)) {
		mulUpdate(start, end, t->left, mul);	
	}
	if (t->right && crossTree(start, end, t->right)) {
		mulUpdate(start, end, t->right, mul);			
	}
	if (t->left) {
		t->sum = t->left->sum + t->right->sum;
		t->sum %= p;
	}
}

void addUpdate(int start, int end, Tree t, LL add) {
	if (t == NULL) return;
	if (containsTree(start, end, t)) {
		t->sum += add * len(t);
		t->sum %= p;
		addUpdateTags(t, add); //更新标记
		return;
	}
	tagDown(t);
	if (t->left && crossTree(start, end, t->left)) {
		addUpdate(start, end, t->left, add);
	}
	if (t->right && crossTree(start, end, t->right)) {
		addUpdate(start, end, t->right, add);
	}
	if (t->left) {
		t->sum = t->left->sum + t->right->sum;
		t->sum %= p;
	}
}

void deleteTree(Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	free(t);
}

int main() {
	int n, m;
	scanf("%d %d %lld", &n, &m, &p);
	LL * arr = (LL *)malloc(n * sizeof(LL));
	for (int i = 0; i < n; i++)
		scanf("%lld", arr + i);
	Tree t = buildTree(arr, 0, n - 1);
	free(arr);
	int cmd, x, y;
	LL k;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &cmd, &x, &y);
		x--; y--;
		if (cmd == 1) {
			scanf("%lld", &k);
			mulUpdate(x, y, t, k);
		}
		else if (cmd == 2) {
			scanf("%lld", &k);
			addUpdate(x, y, t, k);
		}
		else {
			printf("%lld\n", query(x, y, t));
		}
	}
	deleteTree(t);
	return 0;
}
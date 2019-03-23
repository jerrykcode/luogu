#include "stdio.h"
#include "stdlib.h"
#include "memory.h"

/* 最大对称二叉子树的结点数 */
int max_subtree_nodes = 0;

typedef struct TNode {
	int val;
	struct TNode *left, *right;
} *Tree;

/* 比较两颗二叉树是否相同，相同返回1，否则返回0。在相同情况下，用p_nodes指针传出二叉树结点数目。*/
int cmp_tree(Tree a, Tree b, int *p_nodes) {
	if (a == NULL && b == NULL) {
		*p_nodes = 0;
		return 1;
	}
	if (a == NULL || b == NULL) {
		return 0;
	}
	int left_nodes, right_nodes;
	if (a->val == b->val && cmp_tree(a->left, b->left, &left_nodes) && cmp_tree(a->right, b->right, &right_nodes)) {
		*p_nodes = left_nodes + right_nodes + 1; /*左子树结点数目+右子树结点数目+1*/
		return 1;
	}
	return 0;
}

/* 返回original_tree左右子树递归交换后的树。*/
Tree swap_tree(Tree original_tree) {
	if (original_tree == NULL) return NULL;
	Tree tree = (Tree)malloc(sizeof(struct TNode));
	tree->val = original_tree->val;
	tree->left = swap_tree(original_tree->right); /* original_tree的右子树交换后赋给tree的左子树 */
	tree->right = swap_tree(original_tree->left); /* original_tree的左子树交换后赋给tree的右子树 */
	int nodes;
	if (cmp_tree(tree, original_tree, &nodes)) { 
		/* tree和original_tree相同，即tree对称 */
		if (nodes > max_subtree_nodes) max_subtree_nodes = nodes;
	}
	return tree;
}

/* 递归删除树 */
void delete_tree(Tree tree) {
	if (tree == NULL) return;
	delete_tree(tree->left);
	delete_tree(tree->right);
	free(tree);
}

int main() {
	int n;
	scanf("%d", &n);
	Tree *tree_list = (Tree *)malloc(n * sizeof(Tree));
	for (int i = 0; i < n; i++) {
		tree_list[i] = (Tree)malloc(sizeof(struct TNode));
		scanf("%d", &(tree_list[i]->val));
	}
	int left_idx, right_idx;
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &left_idx, &right_idx);
		tree_list[i]->left = (left_idx != -1 ? tree_list[left_idx - 1] : NULL);
		tree_list[i]->right = (right_idx != -1 ? tree_list[right_idx - 1] : NULL);
	}
	Tree root = tree_list[0];
	free(tree_list);
	Tree tree = swap_tree(root);
	printf("%d", max_subtree_nodes);
	delete_tree(root);
	delete_tree(tree);
	return 0;
}
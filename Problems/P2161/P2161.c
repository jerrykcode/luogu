#include "stdio.h"
#include "stdlib.h"

int tree_size;
int removed_node_num;

typedef struct TNode {
	int start, end;
	struct TNode * left;
	struct TNode * right;
} * Tree;

Tree newTNode(int start, int end) {
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->start = start;
	t->end = end;
	t->left = t->right = NULL;
	return t;
}

Tree insert(Tree tree, int start, int end);
Tree leftOverlap(Tree tree, int start);
Tree rightOverlap(Tree tree, int end);
void deleteTree(Tree tree);

Tree insert(Tree tree, int start, int end) {
	if (tree == NULL) {
		tree_size++;	
		return newTNode(start, end);
	}
	if (end < tree->start) {
		tree->left = insert(tree->left, start, end);
	}
	else if (start > tree->end) {
		tree->right = insert(tree->right, start, end);
	}
	else {
		if (start < tree->start) {
			tree->left = leftOverlap(tree->left, start);
		}
		if (end > tree->end) {
			tree->right = rightOverlap(tree->right, end);
		}
		tree->start = start;
		tree->end = end;
		removed_node_num++;
	}
	return tree;
}

Tree leftOverlap(Tree tree, int start) {
	if (tree == NULL) return NULL;
	if (tree->end < start) {
		tree->right = leftOverlap(tree->right, start);
		return tree;
	}
	else {
		deleteTree(tree->right);
		tree->right = NULL;
		Tree left = tree->left;
		free(tree);
		removed_node_num++;
		tree_size--;
		return leftOverlap(left, start);
	}
}

Tree rightOverlap(Tree tree, int end) {
	if (tree == NULL) return NULL;
	if (tree->start > end) {
		tree->left = rightOverlap(tree->left, end);
		return tree;
	}
	else {
		deleteTree(tree->left);
		tree->left = NULL;
		Tree right = tree->right;
		free(tree);
		removed_node_num++;
		tree_size--;
		return rightOverlap(right, end);
	}
}

void deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
	removed_node_num++;
	tree_size--;
}

int main() {
	Tree tree = NULL;
	tree_size = 0;
	int n ,start, end;
	scanf("%d", &n);
	char cmd;	
	for (int i = 0; i < n; i++) {
		scanf(" %c", &cmd);
		if (cmd == 'A') {
			scanf("%d %d", &start, &end);
			removed_node_num = 0;
			tree = insert(tree, start, end);
			printf("%d\n", removed_node_num);
		}
		else {
			printf("%d\n", tree_size);
		}
	}
	deleteTree(tree);
	return 0;
}
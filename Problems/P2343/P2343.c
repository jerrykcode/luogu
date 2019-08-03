#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t size;
	struct TNode * left;
	struct TNode * right;
} *Tree;

#define SIZE(tree) (tree ? tree->size : 0)

Tree newTNode(int key) {
	Tree tree = (Tree)malloc(sizeof(struct TNode));
	tree->key = key;
	tree->size = 1;
	tree->left = tree->right = NULL;
	return tree;
}

Tree leftRotate(Tree tree);
Tree rightRotate(Tree tree);

Tree maintain(Tree tree);
Tree maintainLeft(Tree tree);
Tree maintainRight(Tree tree);

Tree insert(Tree tree, int key);
int rank(Tree tree, int n);

void deleteTree(Tree tree);

Tree leftRotate(Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = tree->size + SIZE(k->right) + 1;
	return k;
}

Tree rightRotate(Tree tree) {
	Tree k = tree->left;
	tree->left = k -> right;
	k->right = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = SIZE(k->left) + tree->size + 1;
	return k;
}

Tree maintain(Tree tree) {
	tree = maintainLeft(tree);
	tree = maintainRight(tree);
	return tree;
}

Tree maintainLeft(Tree tree) {
	if (tree == NULL) return tree;
	if (tree->left == NULL) return tree;
	if (tree->left->left && tree->left->left->size > SIZE(tree->right)) {
		tree = rightRotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	if (tree->left->right && tree->left->right->size > SIZE(tree->right)) {
		tree->left = leftRotate(tree->left);
		tree = rightRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

Tree maintainRight(Tree tree) {
	if (tree == NULL) return tree;
	if (tree->right == NULL) return tree;
	if (tree->right->right && tree->right->right->size > SIZE(tree->left)) {
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	if (tree->right->left && tree->right->left->size > SIZE(tree->left)) {
		tree->right = rightRotate(tree->right);
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

Tree insert(Tree tree, int key) {
	if (tree == NULL) {
		return newTNode(key);
	}
	tree->size++;
	if (key < tree->key) {
		tree->left = insert(tree->left, key);
		tree = maintainLeft(tree);
	}
	else {
		tree->right = insert(tree->right, key);
		tree = maintainRight(tree);
	}	
	return tree;
}

int rank(Tree tree, int n) {
	int right_size = SIZE(tree->right);
	if (n == right_size) {
		return tree->key;
	}
	else if (n < right_size) {
		return rank(tree->right, n);
	}
	else {
		return rank(tree->left, n - right_size - 1);
	}
}

void deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
}

int main() {
	Tree tree = NULL;
	int m, q;
	scanf("%d %d", &m, &q);
	int c, n;
	for (int i = 0; i < m; i++) {
		scanf("%d", &n);
		tree = insert(tree, n);
	}	
	for (int i = 0; i < q; i++) {
		scanf("%d %d", &c, &n);
		if (c == 1) {
			printf("%d\n", rank(tree, n - 1));
		}
		else {
			tree = insert(tree, n);			
		}
	}
	deleteTree(tree);
	return 0;
}
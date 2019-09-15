#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t size;
	struct TNode *left, *right;
} *SBT;

#define SIZE(tree) ((tree) ? (tree->size) : 0)

SBT newTNode(int key);
SBT leftRotate(SBT tree);
SBT rightRotate(SBT tree);
SBT maintain(SBT tree);
SBT maintainLeft(SBT tree);
SBT maintainRight(SBT tree);
SBT insert(SBT tree, int key);
void deleteTree(SBT tree);

SBT tree = NULL;
size_t smaller_num;

int main() {
	int n, key;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &key);
		smaller_num = 0;
		tree = insert(tree, key);
		if (i != 0) putchar(' ');
		printf("%d", smaller_num);
	}
	deleteTree(tree);
	return 0;
}

SBT newTNode(int key) {
	SBT tree = (SBT)malloc(sizeof(struct TNode));
	tree->key = key;
	tree->size = 1;
	tree->left = tree->right = NULL;
}

SBT leftRotate(SBT tree) {
	if (!tree || !tree->right) return tree;
	SBT k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = tree->size + SIZE(k->right) + 1;
	return k;
}

SBT rightRotate(SBT tree) {
	if (!tree || !tree->left) return tree;
	SBT k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->size = SIZE(tree->left) + SIZE(tree->right) + 1;
	k->size = SIZE(k->left) + tree->size + 1;
	return k;
}

SBT maintain(SBT tree) {
	tree = maintainLeft(tree);
	return maintainRight(tree);
}

SBT maintainLeft(SBT tree) {
	if (!tree) return tree;
	if (!tree->left) return tree;
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

SBT maintainRight(SBT tree) {
	if (!tree) return tree;
	if (!tree->right) return tree;
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

SBT insert(SBT tree, int key) {
	if (tree == NULL) {
		return newTNode(key);
	}
	tree->size++;
	if (key < tree->key) {
		tree->left = insert(tree->left, key);
		tree = maintainLeft(tree);
	}
	else { //key >= tree->key
		smaller_num += SIZE(tree->left);
		if (key > tree->key) smaller_num++;
		tree->right = insert(tree->right, key);
		tree = maintainRight(tree);
	}
	return tree;
}

void deleteTree(SBT tree) {
	if (!tree) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	free(tree);
}
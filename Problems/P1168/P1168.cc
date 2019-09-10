#include <cstdio>
#include <cstdlib>

class SBT {
public:
	void insert(int key);
	int select(int rank);
	SBT() : tree_(NULL) {

	}
	~SBT() {
		deleteTree(tree_);
	}
private:
	typedef struct TNode {
		int key;
		size_t size;
		struct TNode *left, *right;
		TNode(int key) : key(key), size(1), left(NULL), right(NULL) {}
	} *Tree;

	size_t getSize(Tree tree) { return tree ? tree->size : 0; }

	Tree leftRotate(Tree tree);
	Tree rightRotate(Tree tree);

	Tree maintain(Tree tree);
	Tree maintainLeft(Tree tree);
	Tree maintainRight(Tree tree);

	Tree insert(Tree tree, int key);
	int select(Tree tree, int rank);

	void deleteTree(Tree tree);
private:
	Tree tree_;
};

void SBT::insert(int key) {
	tree_ = insert(tree_, key);
}

int SBT::select(int rank) {
	return select(tree_, rank);
}

SBT::Tree SBT::leftRotate(Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = getSize(tree->left) + getSize(tree->right) + 1;
	k->size = tree->size + getSize(k->right) + 1;
	return k;
}

SBT::Tree SBT::rightRotate(Tree tree) {
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->size = getSize(tree->left) + getSize(tree->right) + 1;
	k->size = getSize(k->left) + tree->size + 1;
	return k;
}

SBT::Tree SBT::maintain(Tree tree) {
	tree = maintainLeft(tree);
	return maintainRight(tree);
}

SBT::Tree SBT::maintainLeft(Tree tree) {
	if (tree == NULL || tree->left == NULL) return tree;
	if (tree->left->left && tree->left->left->size > getSize(tree->right)) {
		tree = rightRotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	if (tree->left->right && tree->left->right->size > getSize(tree->right)) {
		tree->left = leftRotate(tree->left);
		tree = rightRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

SBT::Tree SBT::maintainRight(Tree tree) {
	if (tree == NULL || tree->right == NULL) return tree;
	if (tree->right->right && tree->right->right->size > getSize(tree->left)) {
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	if (tree->right->left && tree->right->left->size > getSize(tree->left)) {
		tree->right = rightRotate(tree->right);
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

SBT::Tree SBT::insert(Tree tree, int key) {
	if (tree == NULL) {
		return new TNode(key);
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

int SBT::select(Tree tree, int rank) {
	size_t left_size = getSize(tree->left);
	if (rank == left_size) {
		return tree->key;
	}
	else if (rank < left_size) {
		return select(tree->left, rank);
	}
	else {
		return select(tree->right, rank - left_size - 1);
	}
}

void SBT::deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	delete tree;
}
int main() {
	int n, key;
	SBT sbt;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &key);
		sbt.insert(key);
		if (i % 2) {
			printf("%d\n", sbt.select(i / 2));
		}
	}
	return 0;
}
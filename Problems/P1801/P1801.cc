#include <cstdio>
#include <cstdlib>

class SBT {
public:
	SBT() : tree(NULL) {}
	~SBT() {
		deleteTree(tree);
	}
	void insert(int x);
	int select(int k);
private:
	typedef struct TNode {
		int key;
		size_t size;
		struct TNode *left, *right;
		TNode(int key) : key(key), size(1), left(NULL), right(NULL) {}
	} *Tree;
	size_t getSize(Tree t) { return t ? t->size : 0; }
	Tree leftRotate(Tree t);
	Tree rightRotate(Tree t);
	Tree maintain(Tree t);
	Tree maintainLeft(Tree t);
	Tree maintainRight(Tree t);
	Tree insert(Tree t, int key);
	int select(Tree t, int k);
	void deleteTree(Tree t);

	Tree tree;
};

class BlackBox {
public:
	BlackBox() : i (0) {

	}
	void add(int x) {
		sbt.insert(x);
	}
	int get() {
		/*	i++;
			sbt.select(i - 1);*/
		return	sbt.select(i++);
	}
private:
	int i;
	SBT sbt;
};

int main() {
	int m, n;
	scanf("%d %d", &m, &n);
	int * a = new int[m];
	int * u = new int[n];
	for (int i = 0; i < m; i++) scanf("%d", a + i);
	for (int i = 0; i < n; i++) scanf("%d", u + i);
	BlackBox bb;
	int i = 0, j = 0;
	while (i < m && j < n) {
		bb.add(a[i]);
		while (i + 1 == u[j]) {
			printf("%d\n", bb.get());
			j++;			
		}
		i++;
	}
	free(a);
	free(u);
	return 0;
}

/* SBT */
//public functions
void SBT::insert(int x) {
	tree = insert(tree, x);
}

int SBT::select(int k) {
	return select(tree, k);
}

//private functions
SBT::Tree SBT::leftRotate(Tree t) {
	Tree k = t->right;
	t->right = k->left;
	k->left = t;
	k->size = t->size;
	t->size = getSize(t->left) + getSize(t->right) + 1;
	return k;
}

SBT::Tree SBT::rightRotate(Tree t) {
	Tree k = t->left;
	t->left = k->right;
	k->right = t;
	k->size = t->size;
	t->size = getSize(t->left) + getSize(t->right) + 1;
	return k;
}

SBT::Tree SBT::maintain(Tree t) {
	t = maintainLeft(t);
	return maintainRight(t);
}

SBT::Tree SBT::maintainLeft(Tree t) {
	if (t == NULL || t->left == NULL) return t;
	if (t->left->left && t->left->left->size > getSize(t->right)) {
		t = rightRotate(t);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	if (t->left->right && t->left->right->size > getSize(t->right)) {
		t->left = leftRotate(t->left);
		t = rightRotate(t);
		t->left = maintain(t->left);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	return t;
}

SBT::Tree SBT::maintainRight(Tree t) {
	if (t == NULL || t->right == NULL) return t;
	if (t->right->right && t->right->right->size > getSize(t->left)) {
		t = leftRotate(t);
		t->left = maintain(t->left);
		t = maintain(t);
	}
	if (t->right->left && t->right->left->size > getSize(t->left)) {
		t->right = rightRotate(t->right);
		t = leftRotate(t);
		t->left = maintain(t->left);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	return t;
}

SBT::Tree SBT::insert(Tree t, int key) {
	if (t == NULL) {
		return new TNode(key);
	}
	t->size++;
	if (key < t->key) {
		t->left = insert(t->left, key);
		t = maintainLeft(t);
	}
	else {
		t->right = insert(t->right, key);
		t = maintainRight(t);
	}
	return t;
}

int SBT::select(Tree t, int k) {
	size_t left_size = getSize(t->left);
	if (k == left_size) {
		return t->key;
	}
	else if (k < left_size) {
		return select(t->left, k);
	}
	else {
		return select(t->right, k - left_size - 1);
	}
}

void SBT::deleteTree(Tree t) {
	if (t == NULL) return;
	if (t->left) deleteTree(t->left);
	if (t->right) deleteTree(t->right);
	delete t;
}
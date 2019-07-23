#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;

class MinGap {
public:

	const int inf = 0x7fffffff;

	MinGap(int n) : n_(n) {
		front_ = new int[n];
		back_ = new int[n];
		fill(front_, front_ + n, inf);
		fill(back_, back_ + n, inf);
	}
	~MinGap() {
		free(front_);
		free(back_);
		priority_queue<int, vector<int>, greater<int> >().swap(gap_queue_);
		priority_queue<int, vector<int>, greater<int> >().swap(breaked_gap_queue_);
	}
	void initialInsert(int i, int k);
	void insert(int i, int k);
	int minGap();
private:
	int n_;
	int * front_;
	int * back_;
	priority_queue<int, vector<int>, greater<int> > gap_queue_;
	priority_queue<int, vector<int>, greater<int> > breaked_gap_queue_;
};

void MinGap::initialInsert(int i, int k) {
	if (i < n_) {
		front_[i] = k;
		if (i > 0) {
			int gap = abs(k - front_[i - 1]);
			gap_queue_.push(gap);
		}
	}
}

void MinGap::insert(int i, int k) {
	if (i >= n_) return;
	int pre = back_[i] == inf ? front_[i] : back_[i];
	if (i + 1 < n_) {
		breaked_gap_queue_.push(abs(front_[i + 1] - pre));
		gap_queue_.push(abs(front_[i + 1] - k));
	}
	gap_queue_.push(abs(k - pre));
	back_[i] = k;
}

int MinGap::minGap() {
	int min_gap;
	while (true) {
		min_gap = gap_queue_.top();
		if (breaked_gap_queue_.empty() || min_gap != breaked_gap_queue_.top()) break;
		gap_queue_.pop();
		breaked_gap_queue_.pop();
	}
	return min_gap;
}

class MinSortGap {
public:
	MinSortGap() : tree_(NULL), min_sort_gap_(0x7fffffff) {}
	~MinSortGap() {
		deleteTree(tree_);
	}

	void insert(int k);
	int minSortGap();
private:
	typedef struct TNode {
		int key;
		size_t height;
		TNode * left;
		TNode * right;
		TNode(int key) : key(key), height(1), left(NULL), right(NULL) {}
	} *Tree;

	size_t getHeight(Tree tree) { return tree ? tree->height : 0; }

	Tree leftRotate(Tree tree);
	Tree rightRotate(Tree tree);

	Tree insert(Tree tree, int key);

	void deleteTree(Tree tree);
private:
	Tree tree_;
	int min_sort_gap_;
};

MinSortGap::Tree MinSortGap::leftRotate(Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	k->height = max(tree->height, getHeight(k->right)) + 1;
	return k;
}

MinSortGap::Tree MinSortGap::rightRotate(Tree tree) {
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	k->height = max(getHeight(k->left), tree->height) + 1;
	return k;
}

MinSortGap::Tree MinSortGap::insert(Tree tree, int key) {
	if (!min_sort_gap_) return tree;
	if (tree == NULL) {
		return new TNode(key);
	}
	int gap = key - tree->key;
	min_sort_gap_ = min(min_sort_gap_, abs(gap));
	if (!gap) {
		return tree;
	}
	else if (gap < 0) {
		tree->left = insert(tree->left, key);
		if (tree->left->height - getHeight(tree->right) == 2) {
			if (key > tree->left->key) {
				tree->left = leftRotate(tree->left);
			}
			tree = rightRotate(tree);
		}
	}
	else {
		tree->right = insert(tree->right, key);
		if (tree->right->height - getHeight(tree->left) == 2) {
			if (key < tree->right->key) {
				tree->right = rightRotate(tree->right);
			}
			tree = leftRotate(tree);
		}
	}
	tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
	return tree;
}

void MinSortGap::deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	delete tree;
}

void MinSortGap::insert(int k) {
	tree_ = insert(tree_, k);
}

int MinSortGap::minSortGap() {
	return min_sort_gap_;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	MinGap min_gap(n);
	MinSortGap min_sort_gap;
	int idx, k;
	for (int i = 0; i < n; i++) {
		scanf("%d", &k);
		min_gap.initialInsert(i, k);
		min_sort_gap.insert(k);
	}
	string cmd;
	for (int i = 0; i < m; i++) {
		cin >> cmd;
		if (cmd[0] == 'I') {
			scanf("%d %d", &idx, &k);
			min_gap.insert(idx - 1, k);
			min_sort_gap.insert(k);
		}
		else if (cmd[4] == 'G') {
			printf("%d\n", min_gap.minGap());
		}
		else {
			printf("%d\n", min_sort_gap.minSortGap());
		}
	}
	return 0;
}
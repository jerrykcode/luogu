#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

template<typename T>
class SBT {
public:
	void insert(T key);
	void remove(T key);
	int getNodeSize();
	SBT() : tree_(NULL) {}
	~SBT() { deleteTree(tree_); }
private:
	typedef struct TNode {
		T key;
		int num;
		int node_size;
		struct TNode *left, *right;
		TNode(T key) : key(key), num(1), node_size(1), left(NULL), right(NULL) {}
	}*Tree;
	
	int getNodeSize(Tree t) { return t ? t->node_size : 0; }
	Tree leftRotate(Tree t);
	Tree rightRotate(Tree t);
	Tree maintain(Tree t);
	Tree maintainLeft(Tree t);
	Tree maintainRight(Tree t);
	Tree insert(Tree t, T key);
	Tree remove(Tree t, T key);
	void deleteTree(Tree t);
private:
	Tree tree_;
};

struct Query {
	int start, end;
	int index;
	static bool compare(Query a, Query b) {
		return a.start != b.start ? a.start < b.start : a.end < b.end;
	}
};

int main() {
	int n, m;
	scanf("%d", &n);
	int * arr = new int[n];
	for (int i = 0; i < n; i++) {
		scanf("%d", arr + i);
	}
	scanf("%d", &m);
	Query * queries = new Query[m];
	int * ans = new int[m];
	int l, r;
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &l, &r);
		l--; r--;
		queries[i].start = l;
		queries[i].end = r;
		queries[i].index = i;
	}
	sort(queries, queries + m, Query().compare);
	SBT<int> sbt;
	int start = queries[0].start, end = queries[0].end;
	for (int i = start; i <= end; i++)
		sbt.insert(arr[i]);
	for (int i = 0; i < m; i++) {
		for (int j = start; j <= min(end, queries[i].start - 1); j++)
			sbt.remove(arr[j]);
		if (end <= queries[i].end) {
			for (int j = max(end + 1, queries[i].start); j <= queries[i].end; j++)
				sbt.insert(arr[j]);
		}
		else {
			for (int j = queries[i].end + 1; j <= end; j++)
				sbt.remove(arr[j]);
		}
		start = queries[i].start;
		end = queries[i].end;
		ans[queries[i].index] = sbt.getNodeSize();
	}
	for (int i = 0; i < m; i++)
		printf("%d\n", ans[i]);
	free(arr);
	free(queries);
	free(ans);
	return 0;
}

template<typename T>
void SBT<T>::insert(T key) {
	tree_ = insert(tree_, key);
}

template<typename T>
void SBT<T>::remove(T key) {
	tree_ = remove(tree_, key);
}

template<typename T>
int SBT<T>::getNodeSize() {
	return getNodeSize(tree_);
}

template<typename T>
typename SBT<T>::Tree SBT<T>::leftRotate(SBT<T>::Tree t) {
	Tree k = t->right;
	t->right = k->left;
	k->left = t;
	k->node_size = t->node_size;
	t->node_size = getNodeSize(t->left) + getNodeSize(t->right) + 1;
	return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::rightRotate(SBT<T>::Tree t) {
	Tree k = t->left;
	t->left = k->right;
	k->right = t;
	k->node_size = t->node_size;
	t->node_size = getNodeSize(t->left) + getNodeSize(t->right) + 1;
	return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::maintain(SBT<T>::Tree t) {
	t = maintainLeft(t);
	return maintainRight(t);
}

template<typename T>
typename SBT<T>::Tree SBT<T>::maintainLeft(SBT<T>::Tree t) {
	if (t == NULL || t->left == NULL) return t;
	if (t->left->left && t->left->left->node_size > getNodeSize(t->right)) {
		t = rightRotate(t);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	if (t->left->right && t->left->right->node_size > getNodeSize(t->right)) {
		t->left = leftRotate(t->left);
		t = rightRotate(t);
		t->left = maintain(t->left);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::maintainRight(SBT<T>::Tree t) {
	if (t == NULL || t->right == NULL) return t;
	if (t->right->right && t->right->right->node_size > getNodeSize(t->left)) {
		t = leftRotate(t);
		t->left = maintain(t->left);
		t = maintain(t);
	}
	if (t->right->left && t->right->left->node_size > getNodeSize(t->left)) {
		t->right = rightRotate(t->right);
		t = leftRotate(t);
		t->left = maintain(t->left);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::insert(SBT<T>::Tree t, T key) {
	if (t == NULL) {
		return new TNode(key);
	}
	if (key == t->key) {
		t->num++;
	}
	else if (key < t->key) {
		int leftSize = getNodeSize(t->left);
		t->left = insert(t->left, key);
		if (t->left->node_size > leftSize) {
			t->node_size++;
			t = maintainLeft(t);
		}
	}
	else {
		int rightSize = getNodeSize(t->right);
		t->right = insert(t->right, key);
		if (t->right->node_size > rightSize) {
			t->node_size++;
			t = maintainRight(t);
		}
	}
	return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::remove(SBT<T>::Tree t, T key) {
	if (t == NULL) return t;
	if (key == t->key) {
		if (t->num > 1) t->num--;
		else {
			if (t->left && t->right) {
				Tree left_max = t->left;
				while (left_max->right) left_max = left_max->right;
				t->key = left_max->key;
				t->num = left_max->num;
				left_max->key = key;
				left_max->num = 1;
				t->left = remove(t->left, key);
				t = maintainRight(t);
			}
			else {
				Tree tmp = t;
				t = t->left ? t->left : t->right;
				delete tmp;
			}
		}
	}
	else if (key < t->key) {
		int leftSize = getNodeSize(t->left);
		t->left = remove(t->left, key);
		if (t->left->node_size < leftSize) {
			t->node_size--;
			t = maintainRight(t);
		}
	}
	else {
		int rightSize = getNodeSize(t->right);
		t->right = remove(t->right, key);
		if (t->right->node_size < rightSize) {
			t->node_size--;
			t = maintainLeft(t);
		}
	}
	return t;
}

template<typename T>
void SBT<T>::deleteTree(SBT<T>::Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	delete t;
}

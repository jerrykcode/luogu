#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>
using namespace std;

struct Record {
	string name;
	int score;
	int count;
	Record() {}
	Record(string name, int score, int count) : name(name), score(score), count(count) {}
	bool operator< (const Record& rec) {
		return score != rec.score ? score < rec.score : count > rec.count;
	}
	bool operator> (const Record& rec) {
		return score != rec.score ? score > rec.score : count < rec.count;
	}
	bool operator== (const Record& rec) {
		return score == rec.score && count == rec.count;
	}
};

template<typename T>
class SBT {
public:
	SBT() : t(NULL) {}
	~SBT() { deleteTree(t); }
	void insert(T key);
	void remove(T key);
	int rank(T key);
	T rank(int idx);
	int size();
private:
	typedef struct TNode {
		T key;
		int size;
		struct TNode *left;
		struct TNode *right;
		TNode(T key) : key(key), size(1), left(NULL), right(NULL) {}
	} *Tree;

	int getSize(Tree t) { return t ? t->size : 0; }
	
	Tree leftRotate(Tree t);
	Tree rightRotate(Tree t);
	Tree maintain(Tree t);
	Tree maintainLeft(Tree t);
	Tree maintainRight(Tree t);
	Tree insert(Tree t, T key);
	Tree remove(Tree t, T key);
	int rank(Tree t, T key);
	T rank(Tree t, int idx);
	void deleteTree(Tree t);
private:
	Tree t;
};

int main() {
	map<string, Record> name_to_rec;
	SBT<Record> recs;
	int n;
	scanf("%d", &n);
	char cmd;
	string name;
	int score, index;
	for (int i = 0; i < n; i++) {
		scanf(" %c", &cmd);
		if (cmd == '+') {
			cin >> name;
			scanf("%d", &score);
			if (name_to_rec.count(name)) {
				recs.remove(name_to_rec[name]);
			}
			Record rec(name, score, i);
			name_to_rec[name] = rec;
			recs.insert(rec);
		}
		else {
			cin >> name;
			if (name[0] >= 'A' && name[0] <= 'Z') {
				printf("%d\n", recs.rank(name_to_rec[name]));
			}
			else {
				index = stoi(name);
				for (int j = 0; j < 10; j++) {
					if (index + j > recs.size()) break;
					if (j) putchar(' ');
					cout << recs.rank(index + j).name;
				}
				printf("\n");
			}
		}
	}	
	map<string, Record>().swap(name_to_rec);
	return 0;
}

template<typename T>
void SBT<T>::insert(T key) {
	t = insert(t, key);
}

template<typename T>
void SBT<T>::remove(T key) {
	t = remove(t, key);
}

template<typename T>
int SBT<T>::rank(T key) {
	return rank(t, key) + 1;
}

template<typename T>
T SBT<T>::rank(int idx) {
	return rank(t, idx - 1);
}

template<typename T>
int SBT<T>::size() {
	return t->size;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::leftRotate(SBT<T>::Tree t) {
	Tree k = t->right;
	t->right = k->left;
	k->left = t;
	k->size = t->size;
	t->size = getSize(t->left) + getSize(t->right) + 1;
	return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::rightRotate(SBT<T>::Tree t) {
	Tree k = t->left;
	t->left = k->right;
	k->right = t;
	k->size = t->size;
	t->size = getSize(t->left) + getSize(t->right) + 1;
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
	if (t->left->left && t->left->left->size > getSize(t->right)) {
		t = rightRotate(t);
		t->right = maintain(t->right);
		t = maintain(t);
	}
	else if (t->left->right && t->left->right->size > getSize(t->right)) {
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
	if (t->right->right && t->right->right->size > getSize(t->left)) {
		t = leftRotate(t);
		t->left = maintain(t->left);
		t = maintain(t);
	}
	else if (t->right->left && t->right->left->size > getSize(t->left)) {
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


template<typename T>
typename SBT<T>::Tree SBT<T>::remove(SBT<T>::Tree t, T key) {
	t->size--;
	if (key < t->key) {
		t->left = remove(t->left, key);
		t = maintainRight(t);
	}
	else if (key > t->key) {
		t->right = remove(t->right, key);
		t = maintainLeft(t);
	}
	else {
		if (t->left && t->right) {
			Tree left_max = t->left;
			while (left_max->right) left_max = left_max->right;
			t->key = left_max->key;
			t->left = remove(t->left, left_max->key);
			t = maintainRight(t);
		}
		else {
			Tree tmp = t;
			t = t->left ? t->left : t->right;
			delete tmp;
		}
	}
	return t;
}

template<typename T>
int SBT<T>::rank(SBT<T>::Tree t, T key) {
	if (key < t->key) {
		return rank(t->left, key) + getSize(t->right) + 1;
	}
	else if (key > t->key) {
		return rank(t->right, key);
	}
	else {
		return getSize(t->right);
	}
}

template<typename T>
T SBT<T>::rank(SBT<T>::Tree t, int idx) {
	int right_size = getSize(t->right);
	if (idx > right_size) {
		return rank(t->left, idx - right_size - 1);
	}
	else if (idx < right_size) {
		return rank(t->right, idx);
	}
	else {
		return t->key;
	}
}

template<typename T>
void SBT<T>::deleteTree(SBT<T>::Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	delete t;
}

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

class Cashier {
public:

	Cashier(int min);
	~Cashier();

	void insert(int salary); //I
	void increase(int salary_increment); //A 
	void decrease(int salary_decrement); //S
	int select(int k); //F

private:
	//SBT树
	typedef struct TNode {
		int key; //BST按key值排序，在此题中key为工资
		struct TNode * left;
		struct TNode * right;
		size_t size; //结点数量，SBT根据size维护平衡
		TNode(int key) : key(key), left(NULL), right(NULL), size(1) {}
	} *Tree;

	size_t getSize(Tree tree) {
		return tree ? tree->size : 0;
	}

	Tree leftRotate(Tree tree); //左旋
	Tree rightRotate(Tree tree); //右旋

	Tree maintain(Tree tree); //维护平衡
	Tree maintainLeft(Tree tree); //维护平衡--左子树的孩子的size大于右子树的size
	Tree maintainRight(Tree tree); //维护平衡--右子树的孩子的size大于左子树的size

	Tree insert(Tree tree, int key); //插入
	int select(Tree tree, int rank); //在tree中找到排名rank的结点，即有rank个结点的key小于该结点的key，返回该结点的key
	Tree remove(Tree tree, int key); //删除结点

	void deleteTree(Tree tree); //删除树

private:
	Tree tree; //平衡树
	size_t remove_num; //离开的员工数，即tree中删除的结点数
	int min; //最小工资
	queue<Tree> q; //队列
};

Cashier::Cashier(int min) : tree(NULL), remove_num(0), min(min) {

}

Cashier::~Cashier() {
	deleteTree(tree);
	queue<Tree>().swap(q);
	printf("%d\n", remove_num); //输出离开的员工数
}

void Cashier::insert(int salary) {
	if (salary >= min) 
		tree = insert(tree, salary);
	//else remove_num++; // "如果某个员工的初始工资低于最低工资标准,那么将不计入最后的答案内"
}

void Cashier::increase(int salary_increment) {
	//遍历tree，每个结点增加工资
	if (tree == NULL) return;
	q.push(tree);
	while (!q.empty()) {
		Tree t = q.front();
		q.pop();
		t->key += salary_increment;
		if (t->left) q.push(t->left);
		if (t->right) q.push(t->right);
	}
}

void Cashier::decrease(int salary_decrement) {
	//遍历tree，每个结点减少工资，若减少后的工资低于min，则用vector记录，并在最后删除
	if (tree == NULL) return;
	vector<int> v;
	q.push(tree);
	while (!q.empty()) {
		Tree t = q.front();
		q.pop();
		t->key -= salary_decrement;
		if (t->key < min) {
			v.push_back(t->key);
		}
		if (t->left) q.push(t->left);
		if (t->right) q.push(t->right);
	}
	for (int key : v) {
		tree = remove(tree, key); //删除结点
	}
	remove_num += v.size(); //离开员工数量增加
	v.clear();
}

int Cashier::select(int k) {
	//查询第k多
	if (tree == NULL) return -1;
	if (tree->size >= k)
		return select(tree, tree->size - k); //查询有(tree->size - k)个结点比它小的结点
	else return -1;
}

/*-------------------------SBT树-------------------------*/

Cashier::Tree Cashier::leftRotate(Tree tree) {
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	tree->size = getSize(tree->left) + getSize(tree->right) + 1;
	k->size =tree->size + getSize(k->right) + 1;
	return k;
}

Cashier::Tree Cashier::rightRotate(Tree tree) {
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	tree->size = getSize(tree->left) + getSize(tree->right) + 1;
	k->size = getSize(k->left)+ tree->size + 1;
	return k;
}

Cashier::Tree Cashier::maintain(Tree tree) {
	tree = maintainLeft(tree);
	return maintainRight(tree);
}

Cashier::Tree Cashier::maintainLeft(Tree tree) {
	if (tree == NULL || tree->left == NULL) return tree;
	if (tree->left->left && tree->left->left->size > getSize(tree->right)) {
		//左子树的左子树的size大于右子树的size
		tree = rightRotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	else if (tree->left->right && tree->left->right->size > getSize(tree->right)) {
		//左子树的右子树的size大于右子树的size
		tree->left = leftRotate(tree->left);
		tree = rightRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

Cashier::Tree Cashier::maintainRight(Tree tree) {
	if (tree == NULL || tree->right == NULL) return tree;
	if (tree->right->right && tree->right->right->size > getSize(tree->left)) {
		//右子树的右子树的size大于左子树的size
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	else if (tree->right->left && tree->right->left->size > getSize(tree->left)) {
		//右子树的左子树的size大于左子树的size
		tree->right = rightRotate(tree->right);
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

Cashier::Tree Cashier::insert(Tree tree, int key) {
	if (tree == NULL) {
		return new TNode(key);
	}
	tree->size++;
	if (key < tree->key) {
		tree->left = insert(tree->left, key);
		tree = maintainLeft(tree); //插入左子树后，需要判断左子树的孩子的size是否大于右子树的size
	}
	else {
		tree->right = insert(tree->right, key);
		tree = maintainRight(tree); //插入右子树后，需要判断右子树的孩子的size是否大于左子树的size
	}
	return tree;
}

int Cashier::select(Tree tree, int rank) {
	if (tree == NULL) return 0;
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

Cashier::Tree Cashier::remove(Tree tree, int key) {
	if (tree == NULL) return NULL;
	tree->size--;
	if (key == tree->key) {
		if (tree->left && tree->right) { //左右子树都不为NULL
			//用左子树种最大的代替tree
			Tree left_max = tree->left;
			while (left_max->right) left_max = left_max->right;
			tree->key = left_max->key;
			tree->left = remove(tree->left, left_max->key);
			tree = maintainRight(tree); //在左子树中删除结点后，需要判断右子树的孩子的size是否大于左子树的size
		}
		else {
			Tree tmp = tree;
			tree = tree->left ? tree->left : tree->right;
			delete tmp;
		}
	}
	else if (key < tree->key) {
		tree->left = remove(tree->left, key);
		tree = maintainRight(tree); //在左子树中删除结点后，需要判断右子树的孩子的size是否大于左子树的size
	}
	else {
		tree->right = remove(tree->right, key);
		tree = maintainLeft(tree); //在右子树中删除结点后，需要判断左子树的孩子的size是否大于右子树的size
	}
	return tree;
}

void Cashier::deleteTree(Tree tree) {
	if (tree == NULL) return;
	//递归删除
	deleteTree(tree->left);
	deleteTree(tree->right);
	delete tree;
}

int main() {
	int n, min;
	scanf("%d %d", &n, &min);
	Cashier cashier(min);
	char cmd; int k;
	for (int i = 0; i < n * 2; i++) { //循环n*2次才能输入所有数据
		scanf("%c", &cmd);
		scanf("%d", &k);
		switch (cmd) {
		case 'I' : cashier.insert(k); break;
		case 'A': cashier.increase(k); break;
		case 'S': cashier.decrease(k); break;
		case 'F': {
			int salary = cashier.select(k);
			printf("%d\n", salary);
			break;
		}
		default: break;
		}
	}
	//在析构函数里输出离开的员工数量
	return 0;
}
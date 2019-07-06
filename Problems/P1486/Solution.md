# P1486 [NOI2004]郁闷的出纳员
[原题地址](https://www.luogu.org/problemnew/show/P1486)

## 题目描述
OIER公司是一家大型专业化软件公司，有着数以万计的员工。作为一名出纳员，我的任务之一便是统计每位员工的工资。这本来是一份不错的工作，但是令人郁闷的是，我们的老板反复无常，经常调整员工的工资。如果他心情好，就可能把每位员工的工资加上一个相同的量。反之，如果心情不好，就可能把他们的工资扣除一个相同的量。我真不知道除了调工资他还做什么其它事情。

工资的频繁调整很让员工反感，尤其是集体扣除工资的时候，一旦某位员工发现自己的工资已经低于了合同规定的工资下界，他就会立刻气愤地离开公司，并且再也不会回来了。每位员工的工资下界都是统一规定的。每当一个人离开公司，我就要从电脑中把他的工资档案删去，同样，每当公司招聘了一位新员工，我就得为他新建一个工资档案。

老板经常到我这边来询问工资情况，他并不问具体某位员工的工资情况，而是问现在工资第k多的员工拿多少工资。每当这时，我就不得不对数万个员工进行一次漫长的排序，然后告诉他答案。

好了，现在你已经对我的工作了解不少了。正如你猜的那样，我想请你编一个工资统计程序。怎么样，不是很困难吧？

如果某个员工的初始工资低于最低工资标准,那么将不计入最后的答案内

## 输入输出格式

### 输入格式：
第一行有两个非负整数n和min。n表示下面有多少条命令，min表示工资下界。

接下来的n行，每行表示一条命令。命令可以是以下四种之一：

名称 格式 作用

I命令 I_k 新建一个工资档案，初始工资为k。如果某员工的初始工资低于工资下界，他将立刻离开公司。

A命令 A_k 把每位员工的工资加上k

S命令 S_k 把每位员工的工资扣除k

F命令 F_k 查询第k多的工资

_（下划线）表示一个空格，I命令、A命令、S命令中的k是一个非负整数，F命令中的k是一个正整数。

在初始时，可以认为公司里一个员工也没有。

## 输出格式：

### 输出文件的行数为F命令的条数加一。

对于每条F命令，你的程序要输出一行，仅包含一个整数，为当前工资第k多的员工所拿的工资数，如果k大于目前员工的数目，则输出-1。

输出文件的最后一行包含一个整数，为离开公司的员工的总数。

## 输入输出样例

### 输入样例#1： 
> 9 10  
> I 60  
> I 70  
> S 50  
> F 2  
> I 30  
> S 15  
> A 5  
> F 1  
> F 2  

### 输出样例#1： 
> 10  
> 20  
> -1  
> 2  

## 说明

I命令的条数不超过100000

A命令和S命令的总条数不超过100

F命令的条数不超过100000

每次工资调整的调整量不超过1000

新员工的工资不超过100000

## 思路

使用通过size维护平衡的SBT树。用工资作为搜索树排序的依据。

对于4种命令：

* I k  插入新结点
* A k  遍历SBT，把每个结点的工资加上k
* S k  遍历SBT，把每个结点的工资减少k，若低于min，则将其从SBT中删除
* F k  设SBT的结点数量为size，查询一个结点，有(size - k)个结点的工资小于该结点的工资，输出这个结点的工资。size < k 则输出-1

## 代码

```cpp
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
```




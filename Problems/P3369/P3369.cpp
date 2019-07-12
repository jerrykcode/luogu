#include <cstdio>
#include <cstdlib>

class BalancedTree {
public:
	BalancedTree() : tree(NULL) {}
	~BalancedTree() { deleteTree(tree); }

	void insert(int key);
	void remove(int key);

	/* 返回key的排名，即比key小的数的数量+1 */
	int rank(int key);

	/* 返回排名为rank的数 */
	int select(int rank);

	/* 返回key的前驱 */
	int pre(int key);

	/* 返回key的后继 */
	int next(int key);
private:
	//SBT树
	typedef struct TNode {
		int key;
		int num; //key出现的次数
		int num_size; //树的结点存储的数的数量，即所有结点num之和
		int node_size; //树的结点数量，SBT以此维护平衡
		struct TNode * left;
		struct TNode * right;
		TNode(int key) : key(key), num(1), num_size(1), node_size(1), left(NULL), right(NULL) {}
	} *Tree;

	int getNum(Tree tree) { return tree ? tree->num : 0; }
	int getNumSize(Tree tree) { return tree ? tree->num_size : 0; }
	int getNodeSize(Tree tree) { return tree ? tree->node_size : 0; }

	Tree leftRotate(Tree tree); //左旋
	Tree rightRotate(Tree tree); //右旋

	/* 维护平衡 */
	Tree maintain(Tree tree);

	/* 维护平衡--左子树的孩子的node_size大于右子树的node_size 
	    这里SBT以结点的数量(node_size)维护平衡，而不是数的数量(num_size) */
	Tree maintainLeft(Tree tree);

	/* 维护平衡--右子树的孩子的node_size大于左子树的node_size */
	Tree maintainRight(Tree tree);

	/* 插入结点 */
	Tree insert(Tree tree, int key);

	/* 删除结点 */
	Tree remove(Tree tree, int key);

	/* 这里的rank返回的是比key小的数的数量，没有 + 1 */
	int rank(Tree tree, int key);

	/* 比key的后继小的数的数量，即比key小的数的数量 + key出现的次数 */
	int rankNext(Tree tree, int key);

	/* 返回有rank个数比它小的数 */
	int select(Tree tree, int rank);

	void deleteTree(Tree tree);
private:
	Tree tree;
};

/*----------------------------------------------------SBT---------------------------------------------------------*/
BalancedTree::Tree BalancedTree::leftRotate(Tree tree) {
	//左旋
	Tree k = tree->right;
	tree->right = k->left;
	k->left = tree;
	//num_size和node_size都要更新
	k->num_size = tree->num_size;
	k->node_size = tree->node_size;
	tree->num_size = getNumSize(tree->left) + getNumSize(tree->right) + tree->num;
	tree->node_size = getNodeSize(tree->left) + getNodeSize(tree->right) + 1;
	return k;
}

BalancedTree::Tree BalancedTree::rightRotate(Tree tree) {
	//右旋
	Tree k = tree->left;
	tree->left = k->right;
	k->right = tree;
	//num_size和node_size都要更新
	k->num_size = tree->num_size;
	k->node_size = tree->node_size;
	tree->num_size = getNumSize(tree->left) + getNumSize(tree->right) + tree->num;
	tree->node_size = getNodeSize(tree->left) + getNodeSize(tree->right) + 1;
	return k;
}

BalancedTree::Tree BalancedTree::maintain(Tree tree) {
	tree = maintainLeft(tree);
	tree = maintainRight(tree);
	return tree;
}

BalancedTree::Tree BalancedTree::maintainLeft(Tree tree) {
	if (tree == NULL) return NULL;
	if (tree->left == NULL) return tree;
	if (tree->left->left && tree->left->left->node_size > getNodeSize(tree->right)) { //左子树的左子树的node_size大于右子树的node_size
		tree = rightRotate(tree);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	else if (tree->left->right && tree->left->right->node_size > getNodeSize(tree->right)) { //左子树的右子树的node_size右子树的node_size
		tree->left = leftRotate(tree->left);
		tree = rightRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

BalancedTree::Tree BalancedTree::maintainRight(Tree tree) {
	if (tree == NULL) return NULL;
	if (tree->right == NULL) return tree;
	if (tree->right->right && tree->right->right->node_size > getNodeSize(tree->left)) { //右子树的右子树的node_size大于左子树的node_size
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree = maintain(tree);
	}
	else if (tree->right->left && tree->right->left->node_size > getNodeSize(tree->left)) { //右子树的左子树的node_size大于左子树的node_size
		tree->right = rightRotate(tree->right);
		tree = leftRotate(tree);
		tree->left = maintain(tree->left);
		tree->right = maintain(tree->right);
		tree = maintain(tree);
	}
	return tree;
}

BalancedTree::Tree BalancedTree::insert(Tree tree, int key) {
	if (tree == NULL) {
		return new TNode(key); //新建结点
	}
	if (key == tree->key) { //key已经存在
		//注意不用新建结点！！
		tree->num++; //key出现的次数增加
		tree->num_size++; //tree的结点数量不变，但数的数量增加1
	}
	else if (key < tree->key) { 	
		int node_size = getNodeSize(tree->left); //左子树原来的结点数
		tree->left = insert(tree->left, key);	//递归插入左子树
		tree->num_size++; //数的数量增1
		if (getNodeSize(tree->left) > node_size) { //若左子树的结点数量增加，即插入左子树时新建了结点
			tree->node_size++; //结点数量增1
			tree = maintainLeft(tree); //结点数量改变，则改变了树的结构，需要维护平衡。左子树中新建结点，用maintainLeft维护平衡
		}
	}
	else { //key > tree->key
		//与插入左边对称
		int node_size = getNodeSize(tree->right); //右子树原来的结点数
		tree->right = insert(tree->right, key);	//递归插入右子树
		tree->num_size++; //数的数量增1
		if (getNodeSize(tree->right) > node_size) { //若右子树的结点数量增加，即插入右子树时新建了结点
			tree->node_size++; //结点数量增1
			tree = maintainRight(tree); //结点数量改变，则改变了树的结构，需要维护平衡。右子树中新建结点，用maintainRight维护平衡
		}
	}
	return tree;
}

BalancedTree::Tree BalancedTree::remove(Tree tree, int key) {
	if (tree == NULL) {
		//key不存在，无法删除，这种情况下原树不会改变
		return NULL;
	}
	if (key == tree->key) { //找到key
		if (tree->num > 1) { //若key有多个，这里只删除一个，不会删除结点
			tree->num--; //key的数量减1
			tree->num_size--; //数的数量减1
		}
		//若key只有一个，则删除key会删除一个结点
		else if (tree->left && tree->right) { //若tree的左右子树都存在
			Tree pre = tree->left;
			while (pre->right) pre = pre->right; //tree的前驱，左子树中最大的
			tree->key = pre->key; //用pre代替tree
			tree->num = pre->num; //用pre代替tree
			//此时需要在左子树中删除pre，但注意要把pre这个结点删除			
			if (pre->num > 1) {
				//若pre->num > 1，如果直接调用remove，则并没有删掉结点
				int sub_num = pre->num - 1;
				pre->num = 1;
				pre = tree->left;
				while (pre->right) {
					pre->num_size -= sub_num;
					pre = pre->right;
				}
				pre->num_size -= sub_num;
			}
			//此时pre->num为1，调用remove即可删除结点
			tree->left = remove(tree->left, pre->key);
			tree->num_size--; //数的数量减1
			tree->node_size--; //结点的数量减1
			tree = maintainRight(tree); //左子树中删除了结点，右子树的结点数相对增加了，所以用maintainRight维护平衡
		}
		else {
			Tree tmp = tree;
			tree = tree->left ? tree->left : tree->right; //用子树代替
			delete tmp; //删除
		}
	}
	else if (key < tree->key) {
		int node_size = getNodeSize(tree->left); //左子树原来的结点数量
		int num_size = getNumSize(tree->left); //左子树原来的数的数量
		tree->left = remove(tree->left, key); //递归从左子树中删除
		if (getNumSize(tree->left) < num_size) { //左子树的数的数量减少，即key存在
			tree->num_size--; //数的数量减1
		}
		if (getNodeSize(tree->left) < node_size) { //左子树的结点数量减少，即删除了一个结点
			tree->node_size--; //结点数量减1
			tree = maintainRight(tree); //结点数量减少，改变了树的结构，需要维护平衡。左子树删除结点，右子树的结点数相对增加了，所以用maintainRight维护平衡
		}
	}
	else { //key > tree->key
		int node_size = getNodeSize(tree->right); //右子树原来的结点数量
		int num_size = getNumSize(tree->right); //右子树原来的数的数量
		tree->right = remove(tree->right, key); //递归从右子树中删除
		if (getNumSize(tree->right) < num_size) { //右子树的数的数量减少，即key存在
			tree->num_size--; //数的数量减1
		}
		if (getNodeSize(tree->right) < node_size) { //右子树的结点数量减少，即删除了一个结点
			tree->node_size--; //结点数量减1
			tree = maintainLeft(tree); //结点数量减少，改变了树的结构，需要维护平衡。右子树删除结点，左子树的结点数相对增加了，所以用maintainLeft维护平衡
		}
	}
	return tree;
}

int BalancedTree::rank(Tree tree, int key) {
	if (tree == NULL) return 0;
	if (key == tree->key) {
		//返回左子树的数的数量，即tree中比key小的数的数量
		return getNumSize(tree->left);
	}
	else if (key < tree->key) {
		return rank(tree->left, key);
	}
	else {
		//左子树的数的数量 + tree的key的数量 + 右子树中比key小的数量
		return getNumSize(tree->left) + getNum(tree) + rank(tree->right, key);
	}
}

int BalancedTree::rankNext(Tree tree, int key) {
	if (tree == NULL) return 0;
	if (key == tree->key) {
		//左子树的数的数量 + tree的key的数量
		return getNumSize(tree->left) + getNum(tree);
	}
	else if (key < tree->key) {
		return rankNext(tree->left, key);
	}
	else {
		return getNumSize(tree->left) + getNum(tree) + rankNext(tree->right, key);
	}
}

int BalancedTree::select(Tree tree, int rank) {
	// 返回有rank个数比它小的数
	if (tree == NULL) return 0;
	if (rank < getNumSize(tree->left)) {
		return select(tree->left, rank);
	}
	else if (rank >= getNumSize(tree->left) && rank < getNumSize(tree->left) + getNum(tree)) {
		//大于等于左子树的数的数量 && 小于左子树的数的数量 + tree的key的数量
		return tree->key;
	}
	else {
		return select(tree->right, rank - getNumSize(tree->left) - getNum(tree));
	}
}
void BalancedTree::deleteTree(Tree tree) {
	if (tree == NULL) return;
	if (tree->left) deleteTree(tree->left);
	if (tree->right) deleteTree(tree->right);
	delete tree;
}
/*----------------------------------------------------SBT---------------------------------------------------------*/

void BalancedTree::insert(int key) {
	tree = insert(tree, key);
}

void BalancedTree::remove(int key) {
	tree = remove(tree, key);
}

int BalancedTree::rank(int key) {
	return rank(tree, key) + 1; //注意+1
}

int BalancedTree::select(int rank) {
	return select(tree, rank - 1); //注意rank-1
}

int BalancedTree::pre(int key) { //前驱
	int r = rank(tree, key); //key的排名
	return select(tree, r - 1); //key的前面一个数
}

int BalancedTree::next(int key) {
	int r = rankNext(tree, key); //key的后继的排名
	return select(tree, r); //key的后继
}

int main() {
	BalancedTree bt;
	int n;
	scanf("%d", &n);
	int opt, x;
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &opt, &x);
		switch (opt) {
		case 1: bt.insert(x); break;
		case 2: bt.remove(x); break;
		case 3: printf("%d\n", bt.rank(x)); break;
		case 4: printf("%d\n", bt.select(x)); break;
		case 5: printf("%d\n", bt.pre(x)); break;
		case 6: printf("%d\n", bt.next(x)); break;
		default:break;
		}
	}
	return 0;
}
# P1908 逆序对
[原题地址](https://www.luogu.org/problem/P1908)  
## 题目描述
猫猫TOM和小老鼠JERRY最近又较量上了，但是毕竟都是成年人，他们已经不喜欢再玩那种你追我赶的游戏，现在他们喜欢玩统计。最近，TOM老猫查阅到一个人类称之为“逆序对”的东西，这东西是这样定义的：对于给定的一段正整数序列，逆序对就是序列中ai>aj且i<j的有序对。知道这概念后，他们就比赛谁先算出给定的一段正整数序列中逆序对的数目。
Update:数据已加强。  
## 输入格式
第一行，一个数n，表示序列中有n个数。  
第二行n个数，表示给定的序列。序列中每个数字不超过10^9  
## 输出格式
给定序列中逆序对的数目。  
## 输入输出样例
### 输入 #1
>6  
5 4 2 6 3 1  

### 输出 #1
>11  

### 说明/提示
对于25%的数据，n <= 2500  
对于50%的数据，n <= 4 \* 10^4  
对于所有数据，n <= 5 \* 10^5  
请使用较快的输入输出  
应该不会n方过50万吧 by chen_zhe  
## 思路
求逆序对  
输入每个数时，所有已输入的大于它的数，都与它构成逆序对。  
所以只要在输入每个数时，计算出已输入数据中有多少数大于它，即可求出所有逆序对的个数。  
可以有多种解法  
### 树状数组
1)离散化  
输入数据是int范围的，离散化后最小的数据被改为1，其次改为2，最大数据被改为n，故数据范围被缩小至[1, n]，而这并不影响求逆序对的结果，设rank[i]表示第i个输入的数据从小到大的排名(从1开始)，即`rank[i]是第i个数据离散化后的新值`。  
离散化通过对输入数据排序实现。具体见代码。  
2)树状数组  
用一个长度为n + 1的数组，设为arr，每个元素初始化为0。之后，对于每个离散化后的输入数据rank[i]，将arr[rank[i]]更新为1，那么在更新之前，arr[1]到arr[rank[i] - 1]之和就是已输入数据中小于等于rank[i]的数据的个数。那么用`i`(即此时已输入的所有据个数)，减去这个数，就得到了已输入的数据中大于rank[i]的数据的个数。  
还有一个问题就是如何保证arr[1]到arr[rank[i] - 1]之和是已输入数据中`小于等于`rank[i]的数据的个数，而非`小于`，  
所以在之前离散化排序时，对于相同的数据，`先输入的要排在前面`，这样离散化后，先输入的数据改成的新数据会小于后输入的，即假设输入的第i和第j个数据相同(i < j)，那么rank[i] < rank[j]，那么先更新arr[rank[i]]为1，之后更新arr[rank[j]]时，求和就会把arr[rank[i]]的这个1计算进去，也就是说`小于等于`第j个数据的数据都被计算了。  
OK  
接下来终于说到树状数组了  
树状数组基本操作:  
* 单点更新
* 区间求和  

所以只需要维护arr的`树状数组`bit就可以啦。  
对于rank[i]，通过bit计算arr数组在[1, rank[i] - 1]区间之和，用i减去这个和，再加到所有逆序对的数量上。然后再通过bit更新arr[rank[i]]为1就可以了。所有操作都是基于树状数组bit的，arr数组只是为了理解，代码中并不会真正出现。  
### 代码(树状数组)

```c
#include "stdio.h"
#include "stdlib.h"

int lowbit(int x) {
	return x & -x;
}

void update(int * bit, int len, int idx, int val) {
	for (; idx < len; idx += lowbit(idx)) {
		bit[idx] += val;
	}
}

int sum(int * bit, int idx) {
	int result = 0;
	for (; idx > 0; idx -= lowbit(idx)) {
		result += bit[idx];
	}
	return result;
}

//用于离散化
struct node {
	int val; //值
	int input_rank; //输入次序
};

//用于离散化排序
int compare(const void * a, const void * b) {
	//若val不同，则val小的排在前面
	//否则input_rank小的，即先输入的，排在前面
	return ((struct node *)a)->val != ((struct node *)b)->val ? ((struct node *)a)->val - ((struct node *)b)->val :
		((struct node *)a)->input_rank - ((struct node *)b)->input_rank;
}

int main() {
	int n;
	scanf("%d", &n);
	struct node * node_array = (struct node *)malloc(n * sizeof(struct node));
	for (int i = 0; i < n; i++) {
		scanf("%d", &node_array[i].val); //输入数据值
		node_array[i].input_rank = i; //记录输入次序
	}
	qsort(node_array, n, sizeof(struct node), compare); //排序
	int * rank = (int *)malloc(n * sizeof(int)); 
	for (int i = 0; i < n; i++) {
		//所有数据中第i小的数据值为node_array[i].val
		//它是第node_array[i].input_rank个输入的数据
		//离散化后的新值为i + 1
		//+1是因为新值从1开始累加
		rank[node_array[i].input_rank] = i + 1; 
	}
	free(node_array);
	int len = n + 1;
	int * bit = (int *)malloc(len * sizeof(int)); //树状数组
	for (int i = 1; i < len; i++) bit[i] = 0; //初始化
	long long result = 0; //逆序对总数
	for (int i = 0; i < n; i++) {
		result += (i - sum(bit, rank[i] - 1));
		update(bit, len, rank[i], 1);
	}
	printf("%lld", result);
	free(rank);
	free(bit);
	return 0;
}
```  
### SBT树  
SBT树通过结点数量维护平衡。  
由于SBT树记录了每个结点的size，size是`该结点本身及所有子孙结点的数量`。  
那么对每个输入的数，只需将其插入SBT中。若所有插入的数均不相同，那么每次向树的左子树递归插入时，树本身和右子树的所有结点均大于待插入数，即右子树的size+1都大于待插入数。等到这个数插入完毕时，就已经计算出大于这个数的数的数量了。  
但是输入可能存在相同的数，这里的解决方案是把`相同的数用同一个树结点代表`，这样在树结点中需要使用`key_num`属性记录key出现过几次，而此时`n个树结点所代表的数可能大于n个`，所以树结点还需要增加一个`num_size`属性，用来记录树结点本身及所有子结点`实际代表了多少个数`，而普通SBT树中的`size`属性在这里称为`node_size`属性，它表示该结点本身及所有子孙结点的数量，平衡树依据它来维护平衡。
树结点定义如下  
```c
typedef struct TNode {
	int key;
	size_t key_num, num_size, node_size;
	struct TNode *left, *right;
} *Tree;
```
左右旋转与维护平衡的算法和普通SBT相似，只是旋转后注意更新`key_num`，`num_size`，`node_size`三个属性的值。维护平衡时通过node_size判断是否破坏了平衡。  
插入时需要计算树中有多少数大于待插入数，注意num_size是代表的数的数量。具体见代码。  

### 代码(SBT树)

```c
#include "stdio.h"
#include "stdlib.h"

typedef struct TNode {
	int key;
	size_t key_num, num_size, node_size;
	struct TNode *left, *right;
} *Tree;

size_t getKeyNum(Tree t) { return t ? t->key_num : 0; }
size_t getNumSize(Tree t) { return t ? t->num_size : 0; }
size_t getNodeSize(Tree t) { return t ? t->node_size : 0; }

Tree newTNode(int key); //产生新结点
Tree leftRotate(Tree t); //左旋
Tree rightRotate(Tree t); //右旋
Tree maintain(Tree t); //维护平衡
Tree maintainLeft(Tree t); //维护平衡--判断是否存在并处理左子树的子树的node_size大于右子树的node_size的情况
Tree maintainRight(Tree t);  //维护平衡--判断是否存在并处理右子树的子树的node_size大于左子树的node_size的情况

Tree insert(Tree t, int key); //插入key
int greater_num_; //global变量，用于计算有多少数大于待插入数
void deleteTree(Tree t); //删除内存

int main() {
	int n, key;
	scanf("%d", &n);
	long long result = 0;
	Tree t = NULL;
	for (int i = 0; i < n; i++) {
		scanf("%d", &key);
		greater_num_ = 0;
		t = insert(t, key);
		result += greater_num_;
	}
	printf("%lld", result);
	deleteTree(t);
	return 0;
}

Tree newTNode(int key) {
	Tree t = (Tree)malloc(sizeof(struct TNode));
	t->key = key;
	t->key_num = 1;
	t->num_size = 1;
	t->node_size = 1;
	t->left = NULL;
	t->right = NULL;
	return t;
}

Tree leftRotate(Tree t) {
	Tree k = t->right;
	t->right = k->left;
	k->left = t;
	k->num_size = t->num_size;
	k->node_size = t->node_size;
	t->num_size = getNumSize(t->left) + getNumSize(t->right) + t->key_num;
	t->node_size = getNodeSize(t->left) + getNodeSize(t->right) + 1;
	return k;
}

Tree rightRotate(Tree t) {
	Tree k = t->left;
	t->left = k->right;
	k->right = t;
	k->num_size = t->num_size;
	k->node_size = t->node_size;
	t->num_size = getNumSize(t->left) + getNumSize(t->right) + t->key_num;
	t->node_size = getNodeSize(t->left) + getNodeSize(t->right) + 1;
	return k;
}

Tree maintain(Tree t) {
	t = maintainLeft(t);
	return maintainRight(t);
}

Tree maintainLeft(Tree t) {
	//维护平衡--判断是否存在并处理左子树的子树的node_size大于右子树的node_size的情况
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

Tree maintainRight(Tree t) {
	//维护平衡--判断是否存在并处理右子树的子树的node_size大于左子树的node_size的情况
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

Tree insert(Tree t, int key) {
	if (t == NULL) {
		return newTNode(key);
	}
	t->num_size++; //代表的数量增1
	if (key == t->key) {
		t->key_num++; //重复出现
		greater_num_ += getNumSize(t->right); //右边所有结点代表的数都大于key
	}
	else if (key < t->key) {
	 	//右边所有结点代表的数以及t结点代表的数都大于key
		greater_num_ += getNumSize(t->right) + t->key_num;
		size_t tmp_size = getNodeSize(t->left); //左子树的node_size
		t->left = insert(t->left, key); //递归插入左子树
		if (getNodeSize(t->left) > tmp_size) { //若左子树的node_size变化，即左子树中产生了新结点
			t->node_size++;
			t = maintainLeft(t); //左子树中产生了新结点，需要维护平衡
		}
		//若左子树中没有产生新结点，则树的结构不变，不会破坏平衡
	}
	else {
		size_t tmp_size = getNodeSize(t->right); //右子树的node_size
		t->right = insert(t->right, key); //递归插入右子树
		if (getNodeSize(t->right) > tmp_size) { //若右子树的node_size变化，即右子树中产生了新结点
			t->node_size++;
			t = maintainRight(t); //右子树中产生了新结点，需要维护平衡
		}
		//若右子树中没有产生新结点，则树的结构不变，不会破坏平衡
	}
	return t;
}

void deleteTree(Tree t) {
	if (t == NULL) return;
	deleteTree(t->left);
	deleteTree(t->right);
	free(t);
}
```
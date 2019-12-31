# P1533 可怜的狗狗

[原体地址](https://www.luogu.com.cn/problem/P1533)  
[Github题解](https://github.com/jerrykcode/luogu)  
这道题2019年12月31号晚上写完，但是有段错误，直到2020年1月2号中午才调式完并且AC.  
21世纪20年代首篇题解QAQ  

---

## 题目背景

小卡由于公务需要出差，将新家中的狗狗们托付给朋友嘉嘉，但是嘉嘉是一个很懒的人，他才没那么多时间帮小卡喂狗狗。
题目描述

小卡家有N只狗，由于品种、年龄不同，每一只狗都有一个不同的漂亮值。漂亮值与漂亮的程度成反比（漂亮值越低越漂亮），吃饭时，狗狗们会按顺序站成一排等着主人给食物。

可是嘉嘉真的很懒，他才不肯喂这么多狗呢，这多浪费时间啊，于是他每次就只给第i只到第j只狗中第k漂亮的狗狗喂食（好狠心的人啊）。而且为了保证某一只狗狗不会被喂太多次，他喂的每个区间（i,j）不互相包含。
输入格式

第一行输入两个数n，m，你可以假设n<300001 并且 m<50001；m表示他喂了m次。

第二行n个整数，表示第i只狗的漂亮值为ai。

接下来m行，每行3个整数i,j,k表示这次喂食喂第i到第j只狗中第k漂亮的狗的漂亮值。
输出格式

M行，每行一个整数，表示每一次喂的那只狗漂亮值为多少。  
## 输入输出样例
### 输入 #1
>7 2  
>1 5 2 6 3 7 4  
>1 5 3  
>2 7 1

### 输出 #1
>3  
>2

## 思路

离线+SBT平衡树  
查询排名第k的元素，可以使用根据结点数量维护平衡的SBT平衡树。  

首先对区间排序，  
把第一个区间的元素插入SBT，计算第k小的元素  
然后保留第一个区间与第二个区间的交集，  
删除第一个区间有而第二个区间没有的元素，  
插入第二个区间有而第一个区间没有的元素。  
此时SBT中存储的就是第二个区间的元素啦，然后计算第k小的元素。  
若两个区间没有交集，那么就相当于删除第一个区间的所有元素，然后插入第二个区间的所有元素。  
以此类推第3个，第4个，第5个区间...  
因为区间排序过，且区间无相互包含，所以每个元素最多被插入一次，删除一次。  
  
在每个区间中，第k小的元素，有k-1个元素小于它。  
那么在SBT中递归查询就可以啦。  
实现一个函数，在SBT中查询有k个元素小于它的元素(给这个函数传入k-1即可)  
从根结点开始，若k正好等于左子树的size，那么说明正好有k个元素小于根结点，也就是说根结点就是要找的元素。  
若k小于左子树的size，那么待查询结点一定小于根结点，则递归查找左子树中有k个结点小于它的元素。  
若k大于左子树的size，那么待查询结点一定大于根结点，并且在右子树中还有 k - 左子树的size - 1个结点小于它(-1是因为根结点也小于它)，  
那么递归查找右子树中有 k - 左子树的size - 1个元素小于它的元素。    
(若左子树不存在，左子树的size就是0)  
## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

template<typename T>
class SBT {
public:
	void insert(T key);
	void remove(T key);
	T rank(int k);

	SBT() : tree_(NULL) {}
	~SBT() {
		deleteTree(tree_);
	}
	
private:
	typedef struct TNode {
		T key;
		int size;
		struct TNode *left, *right;
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
	T rank(Tree t, int k);
	void deleteTree(Tree t);

private:
	Tree tree_;
};

struct Query {
	int start, end;
	int k;
	int index;
	static bool compare(Query a, Query b) {
		return a.start < b.start;
	}
};

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	int * arr = new int[n];
	Query * queries = new Query[m];
	for (int i = 0; i < n; i++)
		scanf("%d", arr + i);
	int l, r, k;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &l, &r, &k);
		l--; r--; k--; //l和r减一是为了让下标从0开始，k减一是因为SBT中查询排名第k的元素，也是从0开始排名的。从1开始当然也可以，但我的恶习是从0开始qwq
		queries[i].start = l;
		queries[i].end = r;
		queries[i].k = k;
		queries[i].index = i;
	}
	sort(queries, queries + m, Query::compare);
	int * ans = new int[m];
	SBT<int> sbt;
	int start = 0, end = 0;
	sbt.insert(arr[0]); //初始化
	for (int i = 0; i < m; i++) {
		//start和end表示上一个区间的起止位置
		//queries[i].start和queries[i].end是现在要的区间起止位置
		//for循环外的初始化相当于在最开始增加了[0, 0]区间，目的是为了让代码容易写一点(好像也不一定欸)
		for (int j = start; j <= min(end, queries[i].start - 1); j++) //删除掉上一个区间有而本区间没有的元素，注意这个min可以使得代码在两个区间有无相交的情况下都正常运行
			sbt.remove(arr[j]);
		for (int j = max(end + 1, queries[i].start); j <= queries[i].end; j++) //插入本区有而上一个区间没有的元素，同样注意max
			sbt.insert(arr[j]);
		//至此，SBT中存储的已经是本区间元素了
		start = queries[i].start;
		end = queries[i].end;
		ans[queries[i].index] = sbt.rank(queries[i].k); //查询
	}
	free(arr);
	free(queries);	
	for (int i = 0; i < m; i++)
		printf("%d\n", ans[i]);
	free(ans);
	return 0;
}


//下面都是实现SBT的冗长代码
template<typename T>
void SBT<T>::insert(T key) {
	tree_ = insert(tree_, key);
}

template<typename T>
void SBT<T>::remove(T key) {
	tree_ = remove(tree_, key);
}

template<typename T>
T SBT<T>::rank(int k) {
	return rank(tree_, k);
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
	// (t == NULL) return NULL;
	t->size--;
	if (key == t->key){
		if (t->left && t->right) {
			if (t->left->size > t->right->size) {
				Tree left_max = t->left;
				while (left_max->right) {
					left_max = left_max->right;
				}
				t->key = left_max->key;
				t->left = remove(t->left, left_max->key);
				t = maintainRight(t);
			}
			else {
				Tree right_min = t->right;
				while (right_min->left) {
					right_min = right_min->left;
				}
				t->key = right_min->key;
				t->right = remove(t->right, right_min->key);
				t = maintainLeft(t);
			}
		}
		else {
			Tree tmp = t;
			t = t->left ? t->left : t->right;
			delete tmp;
		}
	}
	else if (key < t->key) {
		t->left = remove(t->left, key);
		t = maintainRight(t);
	}
	else {
		t->right = remove(t->right, key);
		t = maintainLeft(t);
	}
	return t;
}


//从0开始排名，好处是排名k的意思就是有k个元素小于它，从1开始排名那就是k-1了。。
template<typename T>
T SBT<T>::rank(SBT<T>::Tree t, int k) {
	//if (t == NULL) return 0xffffffff;
	int left_size = getSize(t->left);
	if (k == left_size) {
		return t->key;
	}
	else if (k < left_size) {
		return rank(t->left, k);
	}
	else {
		return rank(t->right, k - left_size - 1);
	}
}

template<typename T>
void SBT<T>::deleteTree(SBT<T>::Tree t) {
	if (t == NULL) {
		return;
	}
	deleteTree(t->left);
	deleteTree(t->right);
	delete t;
}

```

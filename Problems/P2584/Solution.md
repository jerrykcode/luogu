# P2584 [ZJOI2006]GameZ游戏排名系统
[原体地址](https://www.luogu.com.cn/problem/P2584)

## 题目描述

GameZ为他们最新推出的游戏开通了一个网站。世界各地的玩家都可以将自己的游戏得分上传到网站上。这样就可以看到自己在世界上的排名。得分越高，排名就越靠前。当两个玩家的名次相同时，先上传记录者优先。由于新游戏的火爆，网站服务器已经难堪重负。为此GameZ雇用了你来帮他们重新开发一套新的核心。

排名系统通常要应付三种请求：上传一条新的得分记录、查询某个玩家的当前排名以及返回某个区段内的排名记录。当某个玩家上传自己最新的得分记录时，他原有的得分记录会被删除。为了减轻服务器负担，在返回某个区段内的排名记录时，最多返回10条记录。
输入格式

文件rank.in的第一行是一个整数n（n>=10）表示请求总数目。接下来n行每行包含了一个请求。请求的具体格式如下：

+Name Score 上传最新得分记录。Name表示玩家名字，由大写英文字母组成，不超过10个字符。Score为最多10位的正整数。

?Name 查询玩家排名。该玩家的得分记录必定已经在前面上传。

?Index 返回自第Index名开始的最多10名玩家名字。Index必定合法，即不小于1，也不大于当前有记录的玩家总数。

输入文件总大小不超过2M。

NOTE：用C++的fstream读大规模数据的效率较低
输出格式

对于每条查询请求，在文件rank.out输出相应结果。对于?Name格式的请求，应输出一个整数表示该玩家当前的排名。对于?Index格式的请求，应在一行中依次输出从第Index名开始的最多10名玩家姓名，用一个空格分隔。
## 输入输出样例

### 输入 #1

> 20  
> +ADAM 1000000  
> +BOB 1000000  
> +TOM 2000000  
> +CATHY 10000000  
> ?TOM  
> ?1  
> +DAM 100000  
> +BOB 1200000  
> +ADAM 900000  
> +FRANK 12340000  
> +LEO 9000000  
> +KAINE 9000000   
> +GRACE 8000000  
> +WALT 9000000  
> +SANDY 8000000  
> +MICK 9000000  
> +JACK 7320000  
> ?2  
> ?5  
?KAINE

### 输出 #1

> 2  
> CATHY TOM ADAM BOB  
> CATHY LEO KAINE WALT MICK GRACE SANDY JACK TOM BOB  
> WALT MICK GRACE SANDY JACK TOM BOB ADAM DAM  
> 4

## 说明/提示

20 +ADAM 1000000 加入ADAM的得分记录

+BOB 1000000 加入BOB的得分记录

+TOM 2000000 加入TOM的得分记录

+CATHY 10000000 加入CATHY的得分记录

?TOM 输出TOM目前排名

?1 目前有记录的玩家总数为4，因此应输出第1名到第4名。

+DAM 100000 加入DAM的得分记录

+BOB 1200000 更新BOB的得分记录

+ADAM 900000 更新ADAM的得分记录（即使比原来的差）

+FRANK 12340000 加入FRANK的得分记录

+LEO 9000000 加入LEO的得分记录

+KAINE 9000000 加入KAINE的得分记录

+GRACE 8000000 加入GRACE的得分记录

+WALT 9000000 加入WALT的得分记录

+SANDY 8000000 加入SANDY的得分记录

+MICK 9000000 加入MICK的得分记录

+JACK 7320000 加入JACK的得分记录

?2 目前有记录的玩家总数为12，因此应输出第2名到第11名。

?5 输出第5名到第13名。

?KAINE 输出KAINE的排名


## 思路
使用结构体
```cpp
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

```
存储每一条记录，重载运算符，score不同时，score大的记录更大，score相同时，count即提交次序小的，先提交的更大。

使用一个map存储名字与记录的对应。
再用一个SBT平衡树存储所有记录。

对于+Name Score操作，通过map查询Name，若有对应的记录，即以前该用户提交过，则从SBT中删除该记录，然后map更新记录，再在SBT中插入新的记录。

对于？Name操作，通过map查询Name对应的记录，由于SBT树存储了每个结点的size(所有后代结点包括自己的个数)，那么在SBT中搜索一条记录时，就可以知道大于该记录的记录数量（向左子树递归时，右子树的所有记录及根结点的记录均大于搜索的记录；搜索到该记录时，所在结点的右子树的所有记录也大于它），该数量+1就是排名。

对于？Index操作，循环Index递增10次（若Index大于总数就提前break），查找排名Index的记录。显然在SBT中有Index - 1条记录大于它，在SBT中写一个函数`rank(Tree t, int idx)`，用于查找在SBT中有idx条记录比它大的记录。（那么调用时传递给idx的就是Index - 1了）。rank函数中，从SBT根结点开始，若右子树的size正好等于idx，那么正好有idx个记录大于根结点，所以返回根结点的记录就可以啦；若右子树的size小于idx，那么光右子树的记录还不够，左子树中还要有`idx - 右子树的size - 1`个记录大于查找的记录，递归查找左子树；若右子树的size大于idx，则查找的记录在右子树中，递归查找右子树。

（若结点为NULL，其size应该为0，在代码使用getSize函数判断）

代码中实现了一个SBT模板类，其实对于任意类型T，只要提供了<, >, ==运算，SBT<T>就是一种数据结构，它可以存储，删除T类型的元素，也可以查找一个T类型元素的排名，以及查找排名为idx的元素。由于本题题意，SBT<T>在删除元素，查找元素排名时，没有考虑元素不存在的情况；查询排名为idx的元素时，也没有考虑idx超过所有元素数量的情况。

SBT<Record>即可用于对记录进行操作。

## 代码

```cpp
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

```
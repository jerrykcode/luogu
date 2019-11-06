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
对于50%的数据，n <= 4 * 10^4  
对于所有数据，n <= 5 * 10^5  
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
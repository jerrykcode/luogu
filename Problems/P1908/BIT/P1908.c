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
# P1576 最小花费

[原题地址](https://www.luogu.org/problemnew/show/P1576)


## 题目描述

在n个人中，某些人的银行账号之间可以互相转账。这些人之间转账的手续费各不相同。给定这些人之间转账时需要从转账金额里扣除百分之几的手续费，请问A最少需要多少钱使得转账后B收到100元。

## 输入输出格式

### 输入格式：

第一行输入两个正整数n,m，分别表示总人数和可以互相转账的人的对数。

以下m行每行输入三个正整数x,y,z，表示标号为x的人和标号为y的人之间互相转账需要扣除z%的手续费 (z<100)。

最后一行输入两个正整数A,B。数据保证A与B之间可以直接或间接地转账。

## 输出格式：

输出A使得B到账100元最少需要的总费用。精确到小数点后8位。

## 输入输出样例

### 输入样例#1： 

> 3 3  
> 1 2 1  
> 2 3 2  
> 1 3 3  
> 1 3  

### 输出样例#1： 

> 103.07153164  

## 说明

1<=n<=2000，m<=100000

## 思路

### 问题：

b需要100元，a至少需要转出多少?

### 边权：

设x到y的边权为z，则：

* 若x向y转账n元，则y收到`n*z`元，(即输入 1 2 3，则1到2的边权为0.97)

* 若x向y转账，且y需要收到n元，则x需要转出`n/z`元

### 对dijkstra改造：

* 终点des向起点src转账，src需要100元

* 顶点的dist值表示，该顶点至少需要转出多少，src才能得到100元

* src的dist值为100

* 收录dist值最小的顶点，设该顶点为min_vertex，其dist值为min_dist，对所有邻接点，设边权为weight，若`min_dist/weight`小于邻接点的dist值则更新(即对于这一个邻接点，经过min_vertex转账到src，花费可以更少)

## C++代码

* 邻接表表示图(对每个顶点用一个vector存储其所有的邻接点)

* dijkstra算法实现封装在MinCost类中

* 顶点改为从0开始编号

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

//邻接点结构体
struct AdjNode {
	int v; //邻接顶点
	double weight; //邻接边权重
	AdjNode(int v, double weight) : v(v), weight(weight) {}
};

//计算最小花费类
class MinCost {
public:
	MinCost() {}
	~MinCost() {}

	/* 返回src需要得到num，des至少需要多少花费。*/
	double minCost(vector<AdjNode> *graph, int nv, int src, int des, double num);
private:
	//优先队列使用的结构体
	struct Node {
		int v;
		double dist;
		Node(int v, double dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator() (Node& a, Node& b) {
			return a.dist > b.dist;
		}
	};

	//类变量
	double *dist_;
	bool *collected_;
};

double MinCost::minCost(vector<AdjNode>* graph, int nv, int src, int des, double num) {
	//dijkstra
	dist_ = new double[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	collected_ = new bool[nv];
	fill(collected_, collected_ + nv, false);
	dist_[src] = num;
	priority_queue<Node, vector<Node>, cmp> q;
	q.push(Node(src, dist_[src]));
	int min_vertex, adj_vertex;
	double adj_weight, tmp_dist;
	while (!q.empty()) {
		min_vertex = q.top().v;
		q.pop();
		if (collected_[min_vertex]) continue;
		collected_[min_vertex] = true;
		if (min_vertex == des) {
			while (!q.empty()) q.pop();
			break;
		}
		for (AdjNode adj_node : graph[min_vertex]) { //遍历邻接点
			adj_vertex = adj_node.v;
			adj_weight = adj_node.weight;
			if (!collected_[adj_vertex]) {
				tmp_dist = dist_[min_vertex] / adj_weight; //adj_vertex经过min_vertex到达src的花费
				if (tmp_dist < dist_[adj_vertex] || dist_[adj_vertex] == NO_VALUE) {
					//更新
					dist_[adj_vertex] = tmp_dist;
					q.push(Node(adj_vertex, dist_[adj_vertex]));
				}
			}
		}
	} //while
	double result = dist_[des];
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	int x, y, z, a, b;
	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &x, &y, &z);
		//改为从0开始编号
		x--; y--;
		graph[x].push_back(AdjNode(y, ((double)(1.0) - (double)(0.01) * z)));
		graph[y].push_back(AdjNode(x, ((double)(1.0) - (double)(0.01) * z)));
	}
	scanf("%d %d", &a, &b);
	a--; b--;
	MinCost min_cost;
	printf("%.8lf", min_cost.minCost(graph, n, b, a, (double)(100.0)));
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
```
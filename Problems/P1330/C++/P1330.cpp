/*
洛谷 P1330 封锁阳光大学(https://www.luogu.org/problemnew/show/P1330)

计算二分图并输出二分图中顶点数目较少的顶点集的顶点数，若没有二分图则输出"Impossible"
*/

#include <iostream>
#include <stdlib.h>
#include "stdio.h"
using namespace std;

typedef unsigned char BYTE;
typedef BYTE Color; //二分图染色使用
#define NO_COLOR 0 //没有颜色
#define BLACK 1
#define WHITE 0xff

//实现队列
typedef struct QNode {
	int val;
	struct QNode *next;
	QNode(int val) : val(val), next(NULL) {}
} *QList;

class Queue {
public:
	Queue() :head(NULL), tail(NULL)  {}
	~Queue() { clear(); }

	void push(int val);
	int pop();
	bool empty();
	void clear();
private:
	QList head, tail;
};

void Queue::push(int val) {
	QList list = new QNode(val);
	if (empty()) {
		head = tail = list;
	}
	else {
		tail->next = list;
		tail = list;
	}
}

int Queue::pop() {
	if (empty()) return 0;
	int result = head->val;
	QList tmp = head;
	head = head->next;
	delete tmp;
	return result;
}

bool Queue::empty() {
	return head == NULL;
}

void Queue::clear() {
	while (!empty()) pop();
}

//主要类
class Crab {
public:
	Crab() {}
	~Crab() {}

	/* 对图进行封锁。graph为图，n为顶点数，pNumCrabs指向的值表示封锁需要的河蟹数。
	成功返回true，无法封锁返回false。*/
	bool block(bool **graph, int n, int *pNumCrabs);

private:

	/* BFS二分图染色，vertex为起点，pMinSetVNum指向的值表示二分图两个顶点集中顶点数目较少的数目。
	成功返回true，无法封锁返回false。*/
	bool bfs(bool **graph, Color *color, int n, int vertex, int *pMinSetVNum);
};

bool Crab::block(bool ** graph, int n, int * pNumCrabs) {
	Color *color = new Color[n];
	fill(color, color + n, NO_COLOR);
	*pNumCrabs = 0;
	bool flag = true;
	for (int i = 0; i < n; i++)
		if (color[i] == NO_COLOR) { //对每个连通分量染色
			int crabs = 0;
			if (bfs(graph, color, n, i, &crabs)) {
				*pNumCrabs += crabs; //在二分图中顶点数目较少的顶点集的顶点上放置河蟹
			}
			else {
				flag = false;
				break;
			}
		}
	free(color);
	return flag;
}

bool Crab::bfs(bool ** graph, Color * color, int n, int vertex, int * pMinSetVNum) {
	*pMinSetVNum = 0;
	int maxSetVNum = 0;
	Queue q;
	color[vertex] = BLACK;
	q.push(vertex);	
	(*pMinSetVNum)++; //假设vertex属于顶点数目较少的顶点集
	bool flag = true;
	while (!q.empty()) {
		int v = q.pop();
		for (int w = 0; w < n; w++)
			if (graph[v][w]) {
				if (color[w] == NO_COLOR) {
					color[w] = color[v] == BLACK ? WHITE : BLACK; //w染为与v相反的颜色
					color[w] == color[vertex] ? (*pMinSetVNum)++ : maxSetVNum++;
					q.push(w);
				}
				else if (color[w] == color[v]) {
					flag = false;
					break;
				}
			}
	}
	q.clear();
	if (flag && (*pMinSetVNum) > maxSetVNum) { //让pMinSetVNum指向更少的数目
		*pMinSetVNum = maxSetVNum;
	}
	return flag;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	bool **graph = new bool*[n];
	for (int i = 0; i < n; i++) {
		graph[i] = new bool[n];
		fill(graph[i], graph[i] + n, false);
	}
	int a, b;
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &a, &b);
		a--;
		b--;
		graph[a][b] = graph[b][a] = true;
	}
	Crab crab;
	int nCrabs;
	if (crab.block(graph, n, &nCrabs)) {
		printf("%d\n", nCrabs);
	}
	else {
		printf("Impossible\n");
	}
	for (int i = 0; i < n; i++)
		free(graph[i]);
	free(graph);
	return 0;
}
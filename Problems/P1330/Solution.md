# P1330 封锁阳光大学
[原题地址](https://www.luogu.org/problemnew/show/P1330)

## 题目描述

曹是一只爱刷街的老曹，暑假期间，他每天都欢快地在阳光大学的校园里刷街。河蟹看到欢快的曹

，感到不爽。河蟹决定封锁阳光大学，不让曹刷街。

阳光大学的校园是一张由N个点构成的无向图，N个点之间由M条道路连接。每只河蟹可以对一个点进

行封锁，当某个点被封锁后，与这个点相连的道路就被封锁了，曹就无法在与这些道路上刷街了。

非常悲剧的一点是，河蟹是一种不和谐的生物，当两只河蟹封锁了相邻的两个点时，他们会发生冲

突。

询问：最少需要多少只河蟹，可以封锁所有道路并且不发生冲突。

## 输入输出格式

### 输入格式：

第一行：两个整数N，M

接下来M行：每行两个整数A，B，表示点A到点B之间有道路相连。

### 输出格式：

仅一行：如果河蟹无法封锁所有道路，则输出“Impossible”，否则输出一个整数，表示最少需要

多少只河蟹。

### 输入样例#1： 

3 3  
1 2  
1 3  
2 3  

### 输出样例#1： 

Impossible  


### 输入样例#2： 

3 2  
1 2  
2 3  

### 输出样例#2： 

1  

## 思路

对于所有道路，若一头有河蟹，而另一头没有河蟹，则所有道路均被封锁，且不会有两只河蟹相邻。所以只要计算出二分图，两个顶点集中，一个集合的所有顶点均有河蟹，另一个集合的所有顶点均没有河蟹即可。若没有二分图，则输出"Impossible"。

题目要求输出最少的河蟹，即二分图中顶点数目较少的顶点集的顶点数。

BFS用两种颜色对二分图染色：初始时所有顶点无颜色，对每一个连通分量，从一个顶点开始，将它染成一种颜色，然后入队。对于出队列的顶点，将它的邻接点中没有颜色的染成与它相反的颜色，若邻接点已染色且颜色与它相同，则无法构成二分图。染成同一种颜色的顶点构成一个集合。记录两种颜色的顶点数目，输出较少的一个。


## 代码

### C

```c
/*
洛谷 P1330 封锁阳光大学(https://www.luogu.org/problemnew/show/P1330)

计算二分图并输出二分图中顶点数目较少的顶点集的顶点数，若没有二分图则输出"Impossible"
*/

#include "stdio.h"
#include "stdlib.h"
#include <memory.h>

typedef unsigned char BYTE;
typedef BYTE Bool;
#define True 1
#define False 0

typedef BYTE Color; //二分图染色使用
#define NO_COLOR 0 //没有颜色
#define BLACK 1
#define WHITE 0xff

//实现队列
typedef struct QNode {
	int val;
	struct QNode *next;
} *QList;

typedef struct Queue {
	QList head;
	QList tail;
} *PQueue;

PQueue initQueue() {
	PQueue pQueue = (PQueue)malloc(sizeof(struct Queue));
	pQueue->head = NULL;
	pQueue->tail = NULL;
	return pQueue;
}

Bool qempty(PQueue pQueue) {
	return pQueue->head == NULL;
}

void qpush(PQueue pQueue, int val) {
	QList list = (QList)malloc(sizeof(struct QNode));
	list->val = val;
	list->next = NULL;
	if (qempty(pQueue)) {
		pQueue->head = pQueue->tail = list;
	}
	else {
		pQueue->tail->next = list;
		pQueue->tail = list;
	}
}

int qpop(PQueue pQueue) {
	if (qempty(pQueue)) return 0;
	int result = pQueue->head->val;
	QList head = pQueue->head;
	pQueue->head = head->next;
	free(head);
	return result;
}

/* BFS二分图染色，vertex为起点，pMinSetVNum指向的值表示二分图两个顶点集中顶点数目较少的

数目。
成功返回True，无法封锁返回False。*/
Bool bfs(Bool **graph, Color *color, int n, int vertex, int *pMinSetVNum) {
	*pMinSetVNum = 0;
	int maxSetVNum = 0;
	PQueue pQueue = initQueue();
	color[vertex] = BLACK;
	qpush(pQueue, vertex);	
	(*pMinSetVNum)++; //假设vertex属于顶点数目较少的顶点集
	Bool flag = True;
	while (!qempty(pQueue)) {
		int v = qpop(pQueue);
		for (int w = 0; w < n; w++)
			if (graph[v][w]) {
				if (color[w] == NO_COLOR) {
					color[w] = color[v] == BLACK ? WHITE : BLACK; 

//w染为与v相反的颜色
					color[w] == color[vertex] ? (*pMinSetVNum)++ : 

maxSetVNum++;
					qpush(pQueue, w);
				}
				else if (color[w] == color[v]) {
					flag = False;
					break;
				}
			}
	}
	while (!qempty(pQueue)) qpop(pQueue);
	if (flag && (*pMinSetVNum) > maxSetVNum) { //让pMinSetVNum指向更少的数目
		*pMinSetVNum = maxSetVNum;
	}
	return flag;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	Bool **graph = (Bool **)malloc(n * sizeof(Bool *));
	for (int i = 0; i < n; i++) {
		graph[i] = (Bool *)malloc(n * sizeof(Bool));
		memset(graph[i], False, n * sizeof(Bool));
	}
	int a, b;
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &a, &b);
		a--;
		b--;
		graph[a][b] = graph[b][a] = True;
	}
	Color *color = (Color *)malloc(n * sizeof(Color));
	memset(color, NO_COLOR, n * sizeof(Color));
	int totalCrabs = 0;
	for (int i = 0; i < n; i++) 
		if (color[i] == NO_COLOR) { //对每个连通分量染色
			int crabs;
			if (bfs(graph, color, n, i, &crabs))
				totalCrabs += crabs; //在二分图中顶点数目较少的顶点集的

顶点上放置河蟹
			else {
				printf("Impossible\n");
				totalCrabs = -1;
				break;
			}
		}
	if (totalCrabs != -1) {
		printf("%d\n", totalCrabs);
	}
	free(color);
	for (int i = 0; i < n; i++)
		free(graph[i]);
	free(graph);
	return 0;
}
```

### C++

```cpp
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

	/* BFS二分图染色，vertex为起点，pMinSetVNum指向的值表示二分图两个顶点集中顶点数

目较少的数目。
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
				*pNumCrabs += crabs; //在二分图中顶点数目较少的顶点集的

顶点上放置河蟹
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
					color[w] = color[v] == BLACK ? WHITE : BLACK; 

//w染为与v相反的颜色
					color[w] == color[vertex] ? (*pMinSetVNum)++ : 

maxSetVNum++;
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
```
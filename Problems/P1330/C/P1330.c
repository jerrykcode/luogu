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

/* BFS二分图染色，vertex为起点，pMinSetVNum指向的值表示二分图两个顶点集中顶点数目较少的数目。
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
					color[w] = color[v] == BLACK ? WHITE : BLACK; //w染为与v相反的颜色
					color[w] == color[vertex] ? (*pMinSetVNum)++ : maxSetVNum++;
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
				totalCrabs += crabs; //在二分图中顶点数目较少的顶点集的顶点上放置河蟹
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
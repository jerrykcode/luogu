#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include "stdint.h"
#include <iostream>
using namespace std;

#define HARD '#'
#define SOFT 'x'
#define EMPTY '*'

//格子
struct Grid {
	char c;
	uint16_t row;
	uint16_t col;
};

typedef uint16_t vertex;
#define NOT_A_VERTEX 0xffff

size_t hungarian(vector<vertex> * graph, size_t left_num, size_t right_num) {
	//二分图匹配 匈牙利算法(BFS实现) 
	size_t result = 0;
	vertex * left_matching = new vertex[left_num];
	fill(left_matching, left_matching + left_num, NOT_A_VERTEX);
	vertex * right_matching = new vertex[right_num];
	fill(right_matching, right_matching + right_num, NOT_A_VERTEX);
	vertex * pre = new vertex[left_num];
	fill(pre, pre + left_num, NOT_A_VERTEX);
	bool * visited = new bool[right_num];
	queue<vertex> vq;
	vertex front_v, adj_v, from, to, tmp;
	for (vertex v = 0; v < left_num; v++) {
		fill(visited, visited + right_num, false);
		vq.push(v);
		while (!vq.empty()) {
			front_v = vq.front();
			vq.pop();
			for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) {
				adj_v = *it;
				if (!visited[adj_v]) {
					visited[adj_v] = true;
					if (right_matching[adj_v] != NOT_A_VERTEX) {
						vq.push(right_matching[adj_v]);
						pre[right_matching[adj_v]] = front_v;
					}
					else {
						from = front_v;
						to = adj_v;
						while (from != NOT_A_VERTEX) {
							tmp = left_matching[from];
							left_matching[from] = to;
							right_matching[to] = from;
							from = pre[from];
							to = tmp;
						}
						while (!vq.empty()) vq.pop();
						goto NEXT;
					}
				}
			} //for auto
		} //while
	NEXT:
		if (left_matching[v] != NOT_A_VERTEX) result++;
	} //for (vertex v = 0; v < left_num; v++)
	free(left_matching);
	free(right_matching);
	free(pre);
	free(visited);
	queue<vertex>().swap(vq);
	return result;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	Grid ** grids = new Grid*[n];
	size_t i, j;
	uint16_t n_rows = 0, n_cols = 0;
	for (i = 0; i < n; i++) {
		grids[i] = new Grid[m];
		for (j = 0; j < m; j++) {
			cin >> (grids[i][j].c); //输入该格的字符
			if (grids[i][j].c == HARD) 
				n_rows++; //若遇到硬石头，则该格本身不记录行数，而本行在硬石头之后剩下的格子记为下一行
			else grids[i][j].row = n_rows; //记录所在行
		}
		n_rows++; //进入下一行
	}
	for (j = 0; j < m; j++) {  //按列遍历
		for (i = 0; i < n; i++) {
			if (grids[i][j].c == HARD)
				n_cols++; //若遇到硬石头，则该格本身不记录列数，而本列在硬石头之后剩下的格子记为下一列
			else grids[i][j].col = n_cols; //记录所在列
		}
		n_cols++; //进入下一列
	}
	//建立二分图：以行作为左边顶点，列作为右边顶点
	vector<vertex> * graph = new vector<vertex>[n_rows]; //注意是n_rows不是n
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) 
			if (grids[i][j].c == EMPTY) graph[grids[i][j].row].push_back(grids[i][j].col);	 //所在行向所在列连边	
		free(grids[i]);
	}
	free(grids);
	printf("%u", hungarian(graph, n_rows, n_cols)); //输出二分图匹配结果
	for (i = 0; i < n_rows; i++)
		vector<vertex>().swap(graph[i]);
	return 0;
}
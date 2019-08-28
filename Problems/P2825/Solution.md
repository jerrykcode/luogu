# P2825 [HEOI2016/TJOI2016]游戏
[原题地址](https://www.luogu.org/problem/P2825)

## 题目描述
在2016年，佳缘姐姐喜欢上了一款游戏，叫做泡泡堂。简单的说，这个游戏就是在一张地图上放上若干个炸弹，看是否能炸到对手，或者躲开对手的炸弹。在玩游戏的过程中，小H想到了这样一个问题：当给定一张地图，在这张地图上最多能放上多少个炸弹能使得任意两个炸弹之间不会互相炸到。炸弹能炸到的范围是该炸弹所在的一行和一列，炸弹的威力可以穿透软石头，但是不能穿透硬石头。给定一张n\*m的网格地图:其中\*代表空地，炸弹的威力可以穿透，可以在空地上放置一枚炸弹。x代表软石头，炸弹的威力可以穿透，不能在此放置炸弹。\#代表硬石头，炸弹的威力是不能穿透的，不能在此放置炸弹。例如：给出1\*4的网格地图\*xx\*，这个地图上最多只能放置一个炸弹。给出另一个1\*4的网格地图\*x\#\*，这个地图最多能放置两个炸弹。现在小H任意给出一张n\*m的网格地图，问你最多能放置多少炸弹。

## 输入格式

第一行输入两个正整数n,m,n表示地图的行数，m表示地图的列数。1≤n,m≤50。接下来输入n行m列个字符，代表网格地图。\*的个数不超过n\*m个。

## 输出格式

输出一个整数a，表示最多能放置炸弹的个数

## 输入输出样例

### 输入 #1
4 4  
\#\*\*\*  
\*\#\*\*  
\*\*\#\*  
xxx\#

### 输出 #1
5

## 思路

### 二分图匹配

如果没有硬石头，那么只需将行看作是二分图的一边，列看作另一边，然后可以放置炸弹的地方，所在行向所在列连边即可。

本来一行只能放一颗炸弹，但硬石头的出现导致一行被隔开，所以在有硬石头的情况下，可以把一行看成多行。

### 即把行和列依据硬石头的位置分割成多行和多列

如题中示例

\#\*\*\*  
\*\#\*\*  
\*\*\#\*  
xxx\#

每个格子所在的行为:

\#000  
1\#11  
22\#2  
333\#

考虑到硬石头，则每个格子所在的"行"改为:

\#111  
2\#33  
44\#5  
666\#  
即原来第0行被硬石头分割成了第1行(为了编程方便，遇到硬石头就`行编号++`，所以这里编号为第1行，而不是第0行)  
原来第1行被硬石头分割成了第2行和第3行  
原来第2行被硬石头分割成了第4行和第5行  
原来第3行被硬石头分割成了第6行  
\#(硬石头)所在的格子不属于任一行

对列同样操作

初始每个格子所在列:

\#123  
0\#23  
01\#3  
012\#

重新编号后每个格子所在列:

\#246  
1\#46  
13\#6  
135\#  
即原来第0列被硬石头分割成了第1列  
原来第1列被硬石头分割成了第2列和第3列  
原来第2列被硬石头分割成了第4列和第5列  
原来第3列被硬石头分割成了第6列  
\#(硬石头)所在的格子不属于任一列  

代码中使用
```cpp
struct Grid {
	char c;
	uint16_t row;
	uint16_t col;
};
```
记录每个格子在`行和列被重新编号后的`的行和列

用Grid二维数组存储所有格子，输入时即可知道```grids[i][j].c```，

输入时用`n_rows`记录当前行数，初始为0，一行输入完毕时`n_rows++`，在输入中遇到'\#'也要`n_rows++`，
此时的`n_rows`表示的行数已经是根据硬石头重新编号后的行数了。输入时```grids[i][j].row = n_rows```即可。

之后`按列遍历`二维数组，使用`n_cols`记录当前列数，初始为0，一列遍历完毕时`n_cols++`，在遍历中遇到'\#'也要
`n_cols++`，此时的`n_cols`表示的列数已经是根据硬石头重新编号后的列数了。遍历时```girds[i][j].col = n_cols```即可。

之后再次遍历二维数组，`此时每个格子所在的行向所在的列连边即可构建二分图`。
注意这里二分图`左边顶点的数量是n_rows`，由于输入时最后一行结束后`n_rows++`，所以`n_rows`是`行重新编号后最大的行编号+1`，
`右边顶点数量是n_cols`，由于遍历时最后一列结束后`n_cols++`，所以`n_cols`是`列重新编号后最大的列编号+1`。

二分图匹配就不多说了，这里用的是匈牙利算法的BFS实现。为什么不用DFS递归实现啊，因为那样代码就太简单了QWQ

(逃

## 代码

```cpp
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
```

~END
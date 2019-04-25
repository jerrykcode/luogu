# P1491 集合位置
[原题地址](https://www.luogu.org/problemnew/show/P1491)


题目描述
----

每次有大的活动，大家都要在一起"聚一聚"，不管是去好乐迪，还是避风塘，或者汤姆熊，大家都要玩的痛快。还记得心语和花儿在跳舞机上的激情与释放，还记得草草的投篮技艺是如此的高超，还记得狗狗的枪法永远是'S'......还有不能忘了，胖子的歌声永远是让我们惊叫的！！

今天是野猫的生日，所以想到这些也正常，只是因为是上学日，没法一起去玩了。但回忆一下那时的甜蜜总是一种幸福嘛。。。

但是每次集合的时候都会出现问题！野猫是公认的"路盲"，野猫自己心里也很清楚，每次都提前出门，但还是经常迟到，这点让大家很是无奈。后来，野猫在每次出门前，都会向花儿咨询一下路径，根据已知的路径中，总算能按时到了。

现在提出这样的一个问题：给出n个点的坐标，其中第一个为野猫的出发位置，最后一个为大家的集合位置，并给出哪些位置点是相连的。野猫从出发点到达集合点，总会挑一条最近的路走，如果野猫没找到最近的路，他就会走第二近的路。请帮野猫求一下这条第二最短路径长度。

输入输出格式
------

输入格式：

第一行是两个整数n(1<=n<=200)和m，表示一共有n个点和m条路，以下n行每行两个数xi，yi，(-500<=xi,yi<=500),代表第i个点的坐标，再往下的m行每行两个整数pj，qj,(1<=pj,qj<=n)，表示两个点相通。

输出格式：

只有一行包含一个数，为第二最短路线的距离（保留两位小数），如果存在多条第一短路径，则答案就是第一最短路径的长度；如果不存在第二最短路径，输出－1。

输入输出样例
------

输入样例#1：

> 3 3  
> 0 0  
> 1 1  
> 0 2  
> 1 2  
> 1 3  
> 2 3  

输出样例#1：

> 2.83  

说明
--

各个测试点1s

## 思路

A*计算次短路，注意不能有重复的顶点。。

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;
#define NO_VALUE -1
#define MAX_N 200

//坐标
struct Coordinate {
	float x;
	float y;
	float calculate_dist(Coordinate& c) {
		return sqrt((x - c.x) * (x - c.x) + (y - c.y)*(y - c.y));
	}
};

typedef unsigned char Vertex; //0~255
//邻接点
struct AdjNode {
	Vertex adj_v;
	float adj_weight;
	AdjNode(Vertex adj_v, float adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//次短路类
class SecondShortestDist {
public:
	bool get_second_shortest_dist(vector<AdjNode> * graph, int nv, Vertex src, Vertex des, float *p_second_shortest_dist);
private:
	bool dijkstra(vector<AdjNode> * graph, int nv, Vertex src, Vertex des, float *dist);
	bool astar(vector<AdjNode> * graph, int nv, Vertex src, Vertex des, float *h, float *p_second_shortest_dist);

	//优先队列使用的结构体
	struct PriorityNode {
		Vertex v;
		float dist;
		PriorityNode() {}
		PriorityNode(Vertex v, float dist) : v(v), dist(dist) {	}
	};

	//A*优先队列使用的结构体，继承了PriorityNode
	struct AStarNode : PriorityNode {
		float g;
		//f = dist
		bool contain[MAX_N]; //路径上是否包含某顶点
		AStarNode() {}
		AStarNode(Vertex v, float g, float f) : PriorityNode(v, f), g(g) {
			memset(contain, false, MAX_N * sizeof(bool));
			contain[v] = true;
		}

		//此构造函数由node扩展邻接点时调用
		AStarNode(AStarNode& node, Vertex v, float g, float f) : PriorityNode(v, f), g(g) { 
			memcpy(contain, node.contain, MAX_N * sizeof(bool));
			contain[v] = true;
		}
	};

	//优先队列用于比较的结构体
	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};
};

bool SecondShortestDist::get_second_shortest_dist(vector<AdjNode>* graph, int nv, Vertex src, Vertex des, float * p_second_shortest_dist) {
	float *h = new float[nv];
	bool result;
	if (dijkstra(graph, nv, des, src, h)) { //计算反向最短路，作为A*的估价函数
		result = astar(graph, nv, src, des, h, p_second_shortest_dist); //A*
	}
	else result = false;
	free(h);
	return result;
}

//计算最短路
bool SecondShortestDist::dijkstra(vector<AdjNode>* graph, int nv, Vertex src, Vertex des, float * dist) {
	fill(dist, dist + nv, NO_VALUE);
	bool *collected = new bool[nv];
	fill(collected, collected + nv, false);
	dist[src] = 0;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
	q.push(PriorityNode(src, 0));
	Vertex top_v, adj_v;
	float tmp_dist;
	while (!q.empty()) {
		top_v = q.top().v;
		q.pop();
		if (collected[top_v]) continue;
		collected[top_v] = true;
		for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
			adj_v = it->adj_v;
			if (!collected[adj_v]) {
				tmp_dist = dist[top_v] + it->adj_weight;
				if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
					dist[adj_v] = tmp_dist;
					q.push(PriorityNode(adj_v, tmp_dist));
				}
			}
		} //for
	} //while
	free(collected);
	return dist[des] != NO_VALUE;
}

//计算次短路
bool SecondShortestDist::astar(vector<AdjNode>* graph, int nv, Vertex src, Vertex des, float * h, float * p_second_shortest_dist) {
	priority_queue<AStarNode, vector<AStarNode>, cmp> q;
	q.push(AStarNode(src, 0, h[src]));
	AStarNode top_node;
	Vertex top_v, adj_v;
	float top_g, tmp_g;
	int count = 0;	
	while (!q.empty()) {
		top_node = q.top();
		q.pop();
		top_v = top_node.v;
		top_g = top_node.g;
		if (top_v == des) {
			if (++count == 2) { //第二次到达des
				while (!q.empty()) q.pop();
				*p_second_shortest_dist = top_g;
				return true;
			}
		}
		for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
			adj_v = it->adj_v;		
			if (!top_node.contain[adj_v]) {
				tmp_g = top_g + it->adj_weight;
				//由top_node扩展邻接点
				q.push(AStarNode(top_node, adj_v, tmp_g, tmp_g + h[adj_v]));
			}
		}
	}
	return false;
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	Coordinate *coordinates = new Coordinate[n];
	float x, y;
	for (int i = 0; i < n; i++) {
		scanf("%f %f", &x, &y);		
		coordinates[i].x = x;
		coordinates[i].y = y;
	}
	vector<AdjNode> *graph = new vector<AdjNode>[n];
	float dist;
	for (int i = 0, p, q; i < m; i++) {
		scanf("%d %d", &p, &q);
		p--; q--;
		dist = coordinates[p].calculate_dist(coordinates[q]);
		graph[p].push_back(AdjNode(q, dist));
		graph[q].push_back(AdjNode(p, dist));
	}
	free(coordinates);
	SecondShortestDist ssd;
	float second_shortest_dist;
	if (ssd.get_second_shortest_dist(graph, n, 0, n - 1, &second_shortest_dist)) {
		printf("%.2f", second_shortest_dist);
	}
	else printf("-1");
	for (int i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
```
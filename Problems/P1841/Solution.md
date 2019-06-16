# P1841 [JSOI2007]重要的城市
[原题地址](https://www.luogu.org/problemnew/show/P1841)

## 题目描述

参加jsoi冬令营的同学最近发现，由于南航校内修路截断了原来通向计算中心的路，导致去的路程比原先增加了近一公里。而食堂门前施工虽然也截断了原来通向计算中心的路，却没有使路程增加，因为可以找到同样长度的路作替代。其实，问题的关键在于，路截断的地方是交通要点。

同样的情况也出现在城市间的交通中。某些城市如果出了问题，可能会引起其他很多城市的交通不便。另一些城市则影响不到别的城市的交通。jsoi冬令营的同学发现这是一个有趣的问题，于是决定研究这个问题。

他们认为这样的城市是重要的：如果一个城市c被破坏后，存在两个不同的城市a和b（a, b均不等于c），a到b的最短距离增长了（或不通），则城市c是重要的。

jsoi冬令营的同学面对着一张教练组交给他们的城市间交通图，他们希望能找出所有重要的城市。现在就请你来解决这个问题。

## 输入输出格式

### 输入格式：

第一行两个整数N,M,N为城市数，M为道路数

接下来M行，每行三个整数，表示两个城市之间的无向边，以及之间的路的长度

### 输出格式：

一行，按递增次序输出若干的数，表示重要的城市。

## 输入输出样例

### 输入样例#1： 

> 4 4  
> 1 2 1  
> 2 3 1  
> 4 1 2  
> 4 3 2  

### 输出样例#1： 

> 2  

## 说明

30%的数据：N <= 20；

60%的数据：N <= 100；

100%的数据：N <= 200，M <= N*(N - 1)/2，0 < c <= 10000。c即路的长度。

保证不出现重边和自环

感谢@赵昕鹏 和@qq2477259579 提供程序

如果没有点的话需要输出一行

“No important cities.”

去掉引号


## 思路

对顶点src，以其为源进行dijkstra，对于除src外的任意顶点v，可以得到src到v的最短距离，以及src到v的最短路径上v的前驱顶点(即最短路径上v的前一个顶点)，也可以知道前驱是否唯一。若前驱唯一，则表示经过前驱到达v是src到v的唯一最短路径，那么只要删除前驱顶点，src到v的最短距离一定会变大(甚至src无法到达v)，也就是说v的唯一前驱是重要城市。若src到达v的最短路径上v的前驱(上一个顶点)不唯一，所以删除v的可能的前驱中的一个，v还可以通过其他前驱到达，所以这些前驱不一定是重要城市。

遍历除src外的所有顶点，若其前驱唯一(且不是src)，那么该前驱是重要城市。

注意若v的唯一前驱是src，若删除唯一前驱src，src已不存在，那么src到v的最短距离已无意义，所以src不能被判断为重要城市。

以每一个顶点分别作为src进行dijkstra，可以得到所有的重要城市。

对于dijkstra中判断前驱是否唯一，并记录唯一前驱：

* 从优先队列中弹出的顶点设为top_v，当前它与src的距离是最小的

* top_v的邻接点设为adj_v

* 若src到top_v的距离加上top_v到adj_v的距离`小于`之前src到adj_v的最短距离，则：

	* 经过top_v到达adj_v为目前src到达adj_v的唯一最短路径，更新top_v为adj_v的唯一前驱

* 若src到top_v的距离加上top_v到adj_v的距离`等于`之前src到adj_v的最短距离，则：

	* adj_v可以经过top_v到达，但也可以走之前的最短路径，此时adj_v有多个前驱(不需要记录前驱，只要记录adj_v有多个前驱即可)


## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char vertex_t; //顶点类型(输入顶点编号最大200)
typedef unsigned short weight_t; //边权重类型(输入边权最大10000)
typedef unsigned int dist_t; //距离类型(输入边权最大10000, 但两个顶点之间的距离可能超出65535)

//类型可以存储的最大值
#define VERTEX_T_INF 0xff
#define DIST_T_INF 0xffffffff

//邻接点结构体
struct AdjNode {
	vertex_t adj_v; //邻接顶点
	weight_t adj_weight; //邻接边权重
	AdjNode(vertex_t adj_v, weight_t adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//重要城市类
class ImportantCities {
public:
	/* 标记出重要城市 
	 @param graph 图
	 @param n 图顶点数量
	 @param is_important_city 数组(传出)，标记顶点是否为重要城市
	 @retrun void
	 */
	void importantCities(vector<AdjNode> * graph, size_t n, bool * is_important_city);
private:
	//dijkstra中优先队列使用的结构体
	struct PriorityNode {
		vertex_t v;
		dist_t dist;
		PriorityNode(vertex_t v, dist_t dist) : v(v), dist(dist) {}

		//优先队列用于比较的结构体
		struct cmp {
			bool operator() (PriorityNode& a, PriorityNode& b) {
				return a.dist > b.dist;
			}
		};
	};
private:
	//成员变量
	dist_t * dist_; //最短距离
	vertex_t * pre_; //前驱顶点
	bool * collected_; //标记顶点是否收录
	priority_queue<PriorityNode, vector<PriorityNode>, PriorityNode::cmp> pq_; //优先队列
private:
	static const vertex_t multi_previous = VERTEX_T_INF; //表示有多个前驱
};

void ImportantCities::importantCities(vector<AdjNode>* graph, size_t n, bool * is_important_city) {
	fill(is_important_city, is_important_city + n, false); //初始化
	dist_ = new dist_t[n];
	pre_ = new vertex_t[n];
	collected_ = new bool[n];
	vertex_t src;	
	for (size_t i = 0; i < n; i++) { //遍历顶点
		src = (vertex_t)i; //以顶点 i 作为源进行dijkstra
		fill(dist_, dist_ + n, DIST_T_INF); //初始化
		fill(collected_, collected_ + n, false); //初始化
		dist_[src] = 0;
		pq_.push(PriorityNode(src, 0));
		vertex_t top_v, adj_v;
		dist_t tmp_dist;
		while (!pq_.empty()) {
			top_v = pq_.top().v; //top_v出队列，是当前与src的最短距离中最小的
			pq_.pop();
			if (collected_[top_v]) continue;
			collected_[top_v] = true;
			for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
				adj_v = it->adj_v; //邻接顶点
				if (!collected_[adj_v]) {
					tmp_dist = dist_[top_v] + (dist_t)it->adj_weight; //经过top_v到达adj_v的距离
					if (tmp_dist < dist_[adj_v]) { //若经过top_v到达adj_v可以减小距离
						dist_[adj_v] = tmp_dist; //更新
						pre_[adj_v] = top_v; //更新top_v为adj_v的前驱，此时src经过top_v到达adj_v是src到adj_v的最短路径，所以top_v是adj_v的唯一的前驱
						pq_.push(PriorityNode(adj_v, tmp_dist));
					}
					else if (tmp_dist == dist_[adj_v]) { //若经过top_v到达adj_v与之前到达adj_v的最短距离相同
						//此时从src到达adj_v的最短路径有多条，可以经过top_v也可以使用其他的路径，所以此时adj_v的前驱不唯一，故记录pre_[adj_v]为multi_previous
						pre_[adj_v] = multi_previous; //multi_previous表示adj_v的前驱有多个
					}
				}
			} //for
		} //while (dijkstra)
		for (size_t j = 0; j < n; j++)
			if (j != i && pre_[j] != multi_previous && pre_[j] != src) {
				//若j的前驱唯一，则若把它的唯一前驱顶点删除，src到j的最短距离一定会变大，故它的唯一前驱为重要城市
				//注意j的唯一前驱为src的情况下，src不算重要城市，因为若删除src，src到j的最短距离已无意义
				is_important_city[pre_[j]] = true; //标记j为重要城市
			}
	} //for
	//释放内存
	free(dist_);
	free(pre_);
	free(collected_);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	vector<AdjNode> * graph = new vector<AdjNode>[n];
	int v, w, c;
	//插入边
	for (size_t i = 0; i < m; i++) {
		scanf("%d %d %d", &v, &w, &c);
		v--; w--; //顶点改为从0编号
		graph[v].push_back(AdjNode((vertex_t)w, (weight_t)c));
		graph[w].push_back(AdjNode((vertex_t)v, (weight_t)c));
	}
	ImportantCities important_cities;
	bool * is_important_city = new bool[n];
	important_cities.importantCities(graph, n, is_important_city);
	bool flag = true;
	for (size_t i = 0; i < n; i++) 
		if (is_important_city[i]) { //若是重要城市
			flag = false;
			printf("%d ", i + 1);
		}	
	if (flag) printf("No important cities.");
	for (size_t i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	free(is_important_city);
	return 0;
}
```

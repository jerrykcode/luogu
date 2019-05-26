#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short word;
#define WORD_MAX 0xffff

#ifdef INT_MAX
#undef INT_MAX
#endif

#define INT_MAX 0x7fffffff

//邻接点结构体
struct AdjNode {
	word adj_v; //邻接顶点
	int adj_weight; //邻接边权
	AdjNode(word adj_v, int adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//Dinic类
class Dinic {
public:
	/*
	返回从src到des的最大流。
	@param graph 图
	@param nv 顶点数
	@param src 源
	@param des 终点，汇
	@return int 从src到des的最大流
	*/
	int getMaxFlow(vector<AdjNode> * graph, int nv, word src, word des);
private:
	
	/* bfs计算从src到达每个可达顶点的最短距离(无权图)。若des不可达则返回false。*/
	bool bfs(vector<AdjNode> * graph, int nv, word src, word des);

	/* dfs寻找从src到des的增广路径，返回路径上的最小流量。*/
	int dfs(vector<AdjNode> * graph, int nv, word src, word des, int min_flow);

	word * dist_;
	queue<word> vqueue_;
};

int Dinic::getMaxFlow(vector<AdjNode>* graph, int nv, word src, word des) {
	dist_ = new word[nv];
	int max_flow = 0, flow;
	while (bfs(graph, nv, src, des)) {
		while (flow = dfs(graph, nv, src, des, INT_MAX)) {
			max_flow += flow;
		}
	}
	free(dist_);
	return max_flow;
}

bool Dinic::bfs(vector<AdjNode>* graph, int nv, word src, word des) {
	fill(dist_, dist_ + nv, WORD_MAX);
	dist_[src] = 0;
	vqueue_.push(src);
	word front_v, adj_v;
	while (!vqueue_.empty()) {
		front_v = vqueue_.front();
		vqueue_.pop();
		for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) {
			adj_v = it->adj_v;
			if (dist_[adj_v] == WORD_MAX && it->adj_weight != 0) { //若adj_v没有到达过，且front_v到adj_v的边权(流量)不为0
				dist_[adj_v] = dist_[front_v] + 1;
				vqueue_.push(adj_v);
			}
		} //for
	} //while
	return dist_[des] != WORD_MAX;
}

int Dinic::dfs(vector<AdjNode>* graph, int nv, word src, word des, int min_flow) {
	if (src == des) {
		return min_flow;
	}
	word adj_v;
	int adj_weight;
	int tmp_flow;
	bool flag;
	for (auto adj_it = graph[src].begin(); adj_it != graph[src].end(); adj_it++) {
		adj_v = adj_it->adj_v;
		adj_weight = adj_it->adj_weight;
		if (adj_weight != 0 && dist_[adj_v] == dist_[src] + 1) { //若src到adj_v的边权(流量)不为0且adj_v的前一个顶点是src
			if (tmp_flow = dfs(graph, nv, adj_v, des, min(min_flow, adj_weight))) { //递归，若可以到达des，则向下:
				min_flow = tmp_flow; //增广路径上的最小流量
				adj_it->adj_weight -= min_flow;
				flag = true;
				for (auto inverse_it = graph[adj_v].begin(); inverse_it != graph[adj_v].end(); inverse_it++) //寻找反边
					if (inverse_it->adj_v == src) {
						inverse_it->adj_weight += min_flow;
						flag = false;
						break;
					}
				if (flag) { //不存在反边
					graph[adj_v].push_back(AdjNode(src, min_flow));
				}
				return min_flow; //返回最小流量
			}
		}
	} //for
	return 0; //找不到增广路径了
}

int main() {
	int n, m, w;
	word s, t, u, v;
	scanf("%d %d %hd %hd", &n, &m, &s, &t);
	s--; t--;
	vector<AdjNode> * graph = new vector<AdjNode>[n];
	for (size_t i = 0; i < m; i++) {
		scanf("%hd %hd %d", &u, &v, &w);
		u--; v--;
		graph[u].push_back(AdjNode(v, w));
	}
	Dinic dinic;
	int max_flow = dinic.getMaxFlow(graph, n, s, t);
	printf("%d", max_flow);
	for (size_t i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
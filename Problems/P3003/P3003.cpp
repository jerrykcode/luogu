#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

//AdjNode为邻接点结构体 
//用vector<AdjNode> * 表示图
//即图是一个存储vector的数组，对每个顶点用一个vector存储其邻接点信息
//包括邻接点和它与邻接点之间边的权重
struct AdjNode {
	int adj_vertex; //邻接顶点
	int adj_weight; //邻接边权重
	AdjNode(int adj_vertex, int adj_weight) : adj_vertex(adj_vertex), adj_weight(adj_weight) {}
};

//Dijkstra类
class Dijkstra {
public:
	/*
	计算最短距离
	@param graph 图
	@param n 顶点数
	@param src 源
	@param des 终点
	@return int src到des的最短距离
	*/
	int dijkstra(vector<AdjNode> * graph, int n, int src, int des);
private:

	const int inf = 0x7fffffff;

	//优先队列使用的结构体
	struct PriorityNode {
		int vertex;
		int dist;
		PriorityNode(int vertex, int dist) : vertex(vertex), dist(dist) {}
	};

	//比较PriorityNode
	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};

private:
	//类private成员变量
	int *dist_;
	bool *collected_;
	priority_queue<PriorityNode, vector<PriorityNode>, cmp> pq_;
};

int Dijkstra::dijkstra(vector<AdjNode>* graph, int n, int src, int des) {
	//dijkstra模板
	dist_ = new int[n];
	fill(dist_, dist_ + n, inf); //初始化最短距离
	collected_ = new bool[n];
	fill(collected_, collected_ + n, false); //初始化
	dist_[src] = 0;
	pq_.push(PriorityNode(src, 0));
	int top_vertex, adj_vertex;
	int adj_weight, tmp_dist;
	while (!pq_.empty()) {
		top_vertex = pq_.top().vertex; //当前src到top_vertex的最短距离最小
		pq_.pop();
		if (collected_[top_vertex]) continue;
		collected_[top_vertex] = true;
		if (top_vertex == des) {
			//src到des的最短距离已确定
			while (!pq_.empty()) {
				//清空队列
				pq_.pop();
			}
			break;
		}
		for (auto it = graph[top_vertex].begin(); it != graph[top_vertex].end(); it++) {
			//遍历top_vertex的邻接点
			adj_vertex = it->adj_vertex; //邻接点
			if (!collected_[adj_vertex]) {
				adj_weight = it->adj_weight; //top_vertex与adj_vertex之间的边权重
				tmp_dist = dist_[top_vertex] + adj_weight; //经过top_vertex到达adj_vertex的距离
				if (tmp_dist < dist_[adj_vertex]) {
					dist_[adj_vertex] = tmp_dist;
					pq_.push(PriorityNode(adj_vertex, tmp_dist));
				}
			}
		} //for
	} //while
	int result = dist_[des];
	//删除内存
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int c, p, pb, pa1, pa2;
	scanf("%d %d %d %d %d", &c, &p, &pb, &pa1, &pa2);
	pb--;
	pa1--;
	pa2--;
	vector<AdjNode> * graph = new vector<AdjNode>[p];
	int p1, p2, d;
	for (int i = 0; i < c; i++) {
		scanf("%d %d %d", &p1, &p2, &d);
		p1-- ; 
		p2--;
		graph[p1].push_back(AdjNode(p2, d));
		graph[p2].push_back(AdjNode(p1, d));
	}
	Dijkstra dijkstra;
	int dist_pb_pa1 = dijkstra.dijkstra(graph, p, pb, pa1);
	int dist_pb_pa2 = dijkstra.dijkstra(graph, p, pb, pa2);
	int dist_pa1_pa2 = dijkstra.dijkstra(graph, p, pa1, pa2);
	//pb到pa1再到pa2，或者pb先到pa2再到pa1，这两种中小的即答案
	int result = min(dist_pb_pa1 + dist_pa1_pa2, dist_pb_pa2 + dist_pa1_pa2);
	printf("%d\n", result);
	for (int i = 0; i < p; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
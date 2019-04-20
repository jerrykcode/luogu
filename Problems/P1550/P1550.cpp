#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef short Vertex; //顶点
//邻接点结构体
struct AdjNode {
	Vertex v; //邻接点
	int weight; //邻接边权重
	AdjNode(Vertex v, int weight) : v(v), weight(weight) {}
};

//Prim类
class Prim {
public:
	/* 返回最小生成树的权重。*/
	int prim(vector<AdjNode>* graph, int nv);
private:
	//优先队列使用的结构体
	struct PriorityNode {
		Vertex v;
		int dist;
		PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
	};

	struct cmp {
		bool operator() (PriorityNode& a, PriorityNode& b) {
			return a.dist > b.dist;
		}
	};

	//类变量
	int *dist_;
	priority_queue < PriorityNode, vector<PriorityNode>, cmp> q;
};

int Prim::prim(vector<AdjNode>* graph, int nv) {
	//Prim模板
	dist_ = new int[nv];
	fill(dist_, dist_ + nv, NO_VALUE);
	Vertex src = 0;
	dist_[src] = 0;
	q.push(PriorityNode(src, 0));
	int mst_weight = 0; //结果，初始化为0
	Vertex min_v, adj_v;
	int adj_weight;
	while (!q.empty()) {
		min_v = q.top().v;
		q.pop();
		if (min_v != src && dist_[min_v] == 0) continue;
		mst_weight += dist_[min_v];
		dist_[min_v] = 0;
		for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) {
			//遍历邻接点
			adj_v = it->v;
			if (dist_[adj_v]) { //若不在MST内
				adj_weight = it->weight;
				if (adj_weight < dist_[adj_v] || dist_[adj_v] == NO_VALUE) {
					//更新
					dist_[adj_v] = adj_weight;
					q.push(PriorityNode(adj_v, adj_weight));
				}
			}
		}
	}//while
	free(dist_);
	return mst_weight;
}

int main() {
	int n;
	scanf("%d", &n);
	int nv = n + 1; //总顶点数
	vector<AdjNode> *graph = new vector<AdjNode>[nv];
	for (int i = 0, w; i < n; i++) {
		//输入打井费用
		scanf("%d", &w);	
		//与地面(n号顶点)连边
		graph[i].push_back(AdjNode(n, w));
		graph[n].push_back(AdjNode(i, w));		
	}
	for (int i = 0, w; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//输入连通费用
			scanf("%d", &w);
			if (i != j) {
				graph[i].push_back(AdjNode(j, w));
				graph[j].push_back(AdjNode(i, w));
			}
		}
	}
	Prim prim;
	printf("%d", prim.prim(graph, nv));
	for (int i = 0; i < nv; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
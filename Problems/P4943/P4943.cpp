#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short vertex_t; //顶点类型(输入数据最大50000)
typedef unsigned short weight_t; //图的边权重类型(输入数据最大1000)
typedef unsigned int dist_t; //距离类型(边权重最大1000，但两点之间的距离可能超出65535)

//dist_t可能的最大值
#define DIST_T_INF 0xffffffff

//邻接点结构体
struct AdjNode {
	vertex_t adj_v; //邻接顶点
	weight_t adj_weight; //邻接边权重
	AdjNode(vertex_t adj_v, weight_t adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//Dijkstra
class Dijkstra {
public:
	/*
	计算最短距离
	@param graph 图
	@param n 顶点数，顶点编号0~n-1
	@param src 源
	@param des 终点
	@param accessible 数组，表示一个顶点是否可达
	@return dist_t 返回src与des之间的最短距离
	*/
	dist_t minDist(vector<AdjNode>* graph, size_t n, vertex_t src, vertex_t des, bool * accessible);

private:
	//优先队列存储的结构体
	struct PriorityNode {
		vertex_t v; //顶点
		dist_t dist; //源与v的距离
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
	dist_t * dist_; //dist_[v]为当前src与v的最短距离
	bool * collected_; //collected_[v]表示当前v是否收录
	priority_queue<PriorityNode, vector<PriorityNode>, PriorityNode::cmp> pqueue_; //优先队列
};

dist_t Dijkstra::minDist(vector<AdjNode>* graph, size_t n, vertex_t src, vertex_t des, bool * accessible) {
	//dijkstra算法
	dist_ = new dist_t[n];
	fill(dist_, dist_ + n, DIST_T_INF); //初始化
	collected_ = new bool[n];
	fill(collected_, collected_ + n, false); //初始化
	dist_[src] = 0;
	pqueue_.push(PriorityNode(src, 0));
	vertex_t top_v, adj_v;
	dist_t tmp_dist;
	while (!pqueue_.empty()) {
		top_v = pqueue_.top().v; //当前与src距离最小的顶点出队列
		pqueue_.pop();
		if (collected_[top_v]) continue;
		collected_[top_v] = true;
		if (top_v == des) {
			//到达终点
			while (!pqueue_.empty()) pqueue_.pop();
			break;
		}
		for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //遍历邻接点
			adj_v = it->adj_v; //邻接点
			if (!accessible[adj_v]) continue; //若adj_v不可达，则continue
			if (!collected_[adj_v]) { 
				//此时adj_v没被收录，即src到adj_v的最短距离还未确定
				tmp_dist = dist_[top_v] + (dist_t)it->adj_weight; //tmp_dist = src到top_v的距离 + top_v到adj_v的距离
				if (tmp_dist < dist_[adj_v]) {
					//更新
					dist_[adj_v] = tmp_dist;
					pqueue_.push(PriorityNode(adj_v, tmp_dist));
				}
			}
		} //for
	} //while
	//若src不可达des，则dist_[des]为DIST_T_INF
	dist_t result = dist_[des];
	//释放内存
	free(dist_);
	free(collected_);
	return result;
}

int main() {
	int n, m, k;
	scanf("%d %d %d", &n, &m, &k);	
	bool * accessible = new bool[n];
	//初始化所有顶点可达
	fill(accessible, accessible + n, true);
	size_t i;
	if (k) {
		int room;
		for (i = 0; i < k; i++) {
			scanf("%d", &room);
			//该顶点(房间)仅Harry可达，对Ron来说不可达
			//输入顶点编号为 1~n，改为0~n-1
			accessible[room - 1] = false;
		}
	}
	vector<AdjNode> * graph = new vector<AdjNode>[n];
	int a, b, c;
	for (i = 0; i < m; i++) {
		//插入边
		scanf("%d %d %d", &a, &b, &c);
		a--; b--; //顶点编号改为从0开始		
		graph[a].push_back(AdjNode((vertex_t)b, (weight_t)c));
		graph[b].push_back(AdjNode((vertex_t)a, (weight_t)c));
	}	
	scanf("%d %d", &a, &b);
	//des_x和des_y为需要到达的两个目的地
	vertex_t des_x = a - 1, des_y = b - 1; //编号改为从0开始
	Dijkstra dijkstra;
	//先计算Ron到达des_x与des_y的最短距离，因为此时accessible中Ron不可达的顶点已经标记为false了
	dist_t ron_x = dijkstra.minDist(graph, n, 0, des_x, accessible); //Ron到达des_x的最短距离(时间)
	dist_t ron_y = dijkstra.minDist(graph, n, 0, des_y, accessible); //Ron到达des_y的最短距离(时间)
	dist_t harry_x, harry_y, harry_x_y;
	if (k) { //若存在只能Harry走的顶点
		fill(accessible, accessible + n, true); //所有顶点Harry均可达
		harry_x = dijkstra.minDist(graph, n, 0, des_x, accessible); //Harry到达des_x的最短距离(时间)
		harry_y = dijkstra.minDist(graph, n, 0, des_y, accessible); //Harry到达des_y的最短距离(时间)
		harry_x_y = dijkstra.minDist(graph, n, des_x, des_y, accessible); //Harry从x走到y的最短距离(时间)
	}
	else { //若不存在只能Harry走的顶点
		//所有顶点Harry与Ron都可达，Harry到达任意一个顶点的最短距离与Ron到达该顶点的最短距离相同
		harry_x = ron_x;
		harry_y = ron_y;
	}
	dist_t result = DIST_T_INF; //结果
	result = min(result, max(harry_x, ron_y)); //Harry去des_x, Ron去des_y
	result = min(result, max(harry_y, ron_x)); //Harry去des_y, Ron去des_x
	if (k) {
		//des_x，des_y都由Harry一个人去，
		//(Harry先到des_x再到des_y或者先到des_y再到des_x)
		//在存在只有Harry可达的顶点时，这种方式才可能更快
		result = min(result, min(harry_x + harry_x_y, harry_y + harry_x_y));
	}
	printf("%u", result);
	free(accessible);
	for (i = 0; i < n; i++)
		vector<AdjNode>().swap(graph[i]);
	return 0;
}
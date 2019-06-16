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
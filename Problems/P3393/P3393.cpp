#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1
typedef long long LL;

class Escape {
public:
	Escape() {}
	~Escape() {}

	//main中调用的函数
	void escape();
private:
	//dijkstra中优先队列使用的结构体
	struct Node {
		int v;
		LL dist;
		Node(int v, LL dist) : v(v), dist(dist) {}
	};
	struct cmp {
		bool operator () (Node& a, Node& b) {
			return a.dist > b.dist;
		}
	};

	//private成员函数
	void input(); //输入
	void bfs(); //计算危险城市
	LL dijkstra(); //计算最短路径
	void clear(); //释放内存

	//类变量
	int n, m, k, s;
	int p, q;
	int src, des;
	vector<int> *graph;
	int *cost_; //cost_[i]为城市i的费用，即点权
};

void Escape::escape() {
	input();
	bfs();
	printf("%lld", dijkstra());
	clear();
}

void Escape::input() {
	scanf("%d %d %d %d %d %d", &n, &m, &k, &s, &p, &q);
	src = 0, des = n - 1; //改为从0开始编号
	cost_ = new int[n];
	fill(cost_, cost_ + n, p);
	for (int i = 0, ci; i < k; i++) {
		scanf("%d", &ci);
		ci--; //改为从0开始编号
		cost_[ci] = NO_VALUE;
	}
	graph = new vector<int>[n];
	for (int i = 0, ai, bi; i < m; i++) {
		scanf("%d %d", &ai, &bi);
		ai--; bi--; //改为从0开始编号
		graph[ai].push_back(bi);
		graph[bi].push_back(ai);
	}
}

void Escape::bfs() {	
	//被占领城市周围s层内的城市为危险城市
	if (s <= 0) return;
	queue<int> que;
	bool *collected = new bool[n];
	fill(collected, collected + n, false);
	int last, tail; //last为当前最后一个进队列的，tail为上一层的最后一个
	for (int i = 0; i < n; i++) 
		if (cost_[i] == NO_VALUE) {
			//遍历所有被占领城市
			que.push(i);
			collected[i] = true;
			last = tail = i;
		}
	int level = 0; //当前记为0层
	while (!que.empty()) {
		int v = que.front();
		que.pop();
		for (int adj_v : graph[v]) 
			if (!collected[adj_v]) {
				collected[adj_v] = true;				
				que.push(adj_v);
				cost_[adj_v] = q; //记为危险城市
				last = adj_v; //更新最后入队的顶点
			}
		if (v == tail) { //若上一层的最后一个出队列了，则这一层结束
			if (++level >= s) { //层数增加
				while (!que.empty()) que.pop();
				break;
			}
			tail = last; //更新tail为本层最后一个，即last
		}
	}
	free(collected);
	cost_[src] = cost_[des] = 0;
}

LL Escape::dijkstra() {
	//dijkstra计算最短路径
	//这里(a, b)的费用即为b的费用，(b, a)的费用即为a的费用，即
	//(a, b)的边权为b的点权，(b, a)的边权为a的点权
	LL *dist = new LL[n];
	fill(dist, dist + n, NO_VALUE);
	bool *collected = new bool[n];
	fill(collected, collected + n, false);
	dist[src] = 0;
	priority_queue<Node, vector<Node>, cmp> que;
	que.push(Node(src, dist[src]));
	int min_v;
	LL tmp_dist;
	while (!que.empty()) {
		min_v = que.top().v;
		que.pop();
		if (collected[min_v]) continue;
		collected[min_v] = true;
		if (min_v == des) {
			while (!que.empty()) que.pop();
			break;
		}
		for (int adj_v : graph[min_v]) { //遍历邻接点
			if (!collected[adj_v] && cost_[adj_v] != NO_VALUE) {
				tmp_dist = dist[min_v] + cost_[adj_v]; //(min_v, adj_v) 的边权为 adj_v的点权即cost_[adj_v]
				if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
					dist[adj_v] = tmp_dist;
					que.push(Node(adj_v, tmp_dist));
				}
			}
		} //for adj_v
	} //while
	LL result = dist[des];
	free(dist);
	free(collected);
	return result;
}

void Escape::clear() {
	free(cost_);
	for (int i = 0; i < n; i++)
		vector<int>().swap(graph[i]);
}

int main() {
	Escape escape;
	escape.escape();
	return 0;
}
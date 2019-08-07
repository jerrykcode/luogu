#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char vertex_t;
#define NOT_A_VERTEX 0xff

class Hungarian {
public:
	virtual size_t maxMatching(vector<vertex_t> * graph, size_t left_num, size_t right_num) = 0;
};

//DFS
class DFS : public Hungarian {
public:
	virtual size_t maxMatching(vector<vertex_t> * graph, size_t left_num, size_t right_num);
private:
	bool dfs(vertex_t v);
private:
	vector<vertex_t> * graph_;
	size_t left_num_, right_num_;
	bool * visited_;
	vertex_t * matching_;
};

size_t DFS::maxMatching(vector<vertex_t>* graph, size_t left_num, size_t right_num) {
	graph_ = graph;
	left_num_ = left_num;
	right_num_ = right_num;
	visited_ = new bool[right_num];
	matching_ = new vertex_t[right_num];
	fill(matching_, matching_ + right_num, NOT_A_VERTEX);
	size_t result = 0;
	for (vertex_t v = 0; v < left_num; v++) {
		fill(visited_, visited_ + right_num, false);
		if (dfs(v)) result++;
	}
	free(visited_);
	free(matching_);
	return result;
}

bool DFS::dfs(vertex_t v) {
	vertex_t adj_v;
	for (auto it = graph_[v].begin(); it != graph_[v].end(); it++) {
		adj_v = *it;
		if (!visited_[adj_v]) {
			visited_[adj_v] = true;
			if (matching_[adj_v] == NOT_A_VERTEX || dfs(matching_[adj_v])) {
				matching_[adj_v] = v;
				return true;
			}
		}
	}
	return false;
}

class BFS : public Hungarian {
public:
	virtual size_t maxMatching(vector<vertex_t> * graph, size_t left_num, size_t right_num);
private:
	vertex_t * left_matching_;
	vertex_t * right_matching_;
	vertex_t * pre_;
	bool * visited_;
	queue<vertex_t> vqueue_;
};

size_t BFS::maxMatching(vector<vertex_t>* graph, size_t left_num, size_t right_num) {	
	left_matching_ = new vertex_t[left_num];
	fill(left_matching_, left_matching_ + left_num, NOT_A_VERTEX);
	right_matching_ = new vertex_t[right_num];
	fill(right_matching_, right_matching_ + right_num, NOT_A_VERTEX);
	pre_ = new vertex_t[left_num];
	fill(pre_, pre_ + left_num, NOT_A_VERTEX);
	size_t result = 0;
	visited_ = new bool[right_num];
	for (vertex_t v = 0; v < left_num; v++) {
		fill(visited_, visited_ + right_num, false);
		vqueue_.push(v);
		vertex_t front_v, adj_v;
		while (!vqueue_.empty()) {
			front_v = vqueue_.front();
			vqueue_.pop();
			for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) {
				adj_v = *it;
				if (!visited_[adj_v]) {
					visited_[adj_v] = true;
					if (right_matching_[adj_v] != NOT_A_VERTEX) {
						vqueue_.push(right_matching_[adj_v]);
						pre_[right_matching_[adj_v]] = front_v;
					}
					else {
						vertex_t from = front_v, to = adj_v, tmp_v;
						while (from != NOT_A_VERTEX) {
							tmp_v = left_matching_[from];
							left_matching_[from] = to;
							right_matching_[to] = from;
							from = pre_[from];
							to = tmp_v;
						}
						while (!vqueue_.empty()) vqueue_.pop();
						goto NEXT;
					}
				}
			}
		} //while
	NEXT:
		if (left_matching_[v] != NOT_A_VERTEX) result++;
	} //for
	free(left_matching_);
	free(right_matching_);
	free(pre_);
	free(visited_);
	queue<vertex_t>().swap(vqueue_);
	return result;
}

int main() {
	int num, p, n, m, s, i, j, k;
	scanf("%d", &num);
	vector<vertex_t> * graph;
	Hungarian * p_hungarian = new BFS();
	for (i = 0; i < num; i++) {
		scanf("%d %d", &p, &n);
		graph = new vector<vertex_t>[p];
		for (j = 0; j < p; j++) {
			scanf("%d", &m);
			for (k = 0; k < m; k++) {
				scanf("%d", &s);
				s--;
				graph[j].push_back(s);
			}
		}
		if (p_hungarian->maxMatching(graph, p, n) == p) 
			printf("YES\n");
		else printf("NO\n");
	}
	delete p_hungarian;
	return 0;
}
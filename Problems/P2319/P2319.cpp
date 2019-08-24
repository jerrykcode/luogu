#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short vertex;
#define NOT_A_VERTEX 0xffff

void hungarian(vector<vertex> * graph, size_t left_num, size_t right_num) {
	vertex * left_matching = new vertex[left_num];
	fill(left_matching, left_matching + left_num, NOT_A_VERTEX);
	vertex * right_matching = new vertex[right_num];
	fill(right_matching, right_matching + right_num, NOT_A_VERTEX);
	vertex * pre = new vertex[left_num];
	fill(pre, pre + left_num, NOT_A_VERTEX);
	bool * visited = new bool[right_num];
	queue<vertex> vqueue;
	size_t num = 0;
	for (vertex v = 0; v < left_num; v++) {
		fill(visited, visited + right_num, false);
		vqueue.push(v);
		vertex front_v, adj_v, from, to, tmp;
		while (!vqueue.empty()) {
			front_v = vqueue.front();
			vqueue.pop();
			for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) {
				adj_v = *it;
				if (!visited[adj_v]) {
					visited[adj_v] = true;
					if (right_matching[adj_v] != NOT_A_VERTEX) {
						vqueue.push(right_matching[adj_v]);
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
						while (!vqueue.empty()) vqueue.pop();
						goto NEXT;
					}
				}
			} //for auto
		} //BFS while
	NEXT:
		if (left_matching[v] != NOT_A_VERTEX) num++;
		else break;
	} // for (vertex v = 0; v < left_num; v++)
	printf("%d", num);
	for (size_t i = 0; i < left_num; i++) {
		if (left_matching[i] != NOT_A_VERTEX) {
			printf("\n%hu", left_matching[i]);
		}
	}
	free(left_matching);
	free(right_matching);
	free(pre);
	free(visited);
}

int main() {
	unsigned short n, m;
	scanf("%hu %hu", &n, &m);
	vector<vertex> * graph = new vector<vertex>[m];
	vertex a, b;
	for (int i = 0; i < m; i++) {
		scanf("%hu %hu", &a, &b);	
		graph[i].push_back(a);
		if (b != a) graph[i].push_back(b);
	}
	hungarian(graph, m, n);
	for (size_t i = 0; i < m; i++) {
		vector<vertex>().swap(graph[i]);
	}
	return 0;
}
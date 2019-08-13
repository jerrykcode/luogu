#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short vertex;
#define NOT_A_VERTEX 0xffff

size_t hungarian(vector<vertex> * graph, size_t num) {
	vertex * left_matching = new vertex[num];
	fill(left_matching, left_matching + num, NOT_A_VERTEX);
	vertex * pre = new vertex[num];
	fill(pre, pre + num, NOT_A_VERTEX);
	vertex * right_matching = new vertex[num];
	fill(right_matching, right_matching + num, NOT_A_VERTEX);
	bool * visited = new bool[num];
	queue<vertex> vqueue;
	size_t result = 0;
	for (vertex v = 0; v < num; v++) {
		fill(visited, visited + num, false);
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
			} //for auto it
		} //while (!vqueue.empty())  
	NEXT:
		if (left_matching[v] != NOT_A_VERTEX) result++;
	} //for v
	free(left_matching);
	free(pre);
	free(right_matching);
	free(visited);
	queue<vertex>().swap(vqueue);
	return result;
}

int main() {
	int n;
	scanf("%d", &n);
	vector<vertex> * graph = new vector<vertex>[n * 2];
	size_t i;
	for (i = 0; i < n * 2; i++) graph[i].resize(4);
	vertex s1, s2;
	for (i = 0; i < n * 2; i++) {
		scanf("%hu %hu", &s1, &s2);
		s1--; s2--;
		s1 *= 2; s2 *= 2;
		graph[i][0] = s1;
		graph[i][1] = s1 + 1;
		graph[i][2] = s2;
		graph[i][3] = s2 + 1;
	}
	printf("%d", hungarian(graph, n * 2));
	for (i = 0; i < n * 2; i++)
		vector<vertex>().swap(graph[i]);
	return 0;
}
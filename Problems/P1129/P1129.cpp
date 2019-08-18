#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char vertex;
#define MAX_N 200
#define NOT_A_VERTEX 0xff

queue<vertex> vqueue;
bool hungarian(vector<vertex> * graph, int num) {
	vertex * left_matching = new vertex[num];
	fill(left_matching, left_matching + num, NOT_A_VERTEX);
	vertex * right_matching = new vertex[num];
	fill(right_matching, right_matching + num, NOT_A_VERTEX);
	vertex * pre = new vertex[num];
	fill(pre, pre + num, NOT_A_VERTEX);
	bool * visited = new bool[num];	
	bool result = true;
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
			} //for auto
		} //while
	NEXT:
		if (left_matching[v] != NOT_A_VERTEX) continue;
		result = false;
		break;
	} //for (vertex v = 0; v < num; v++) 
	free(left_matching);
	free(right_matching);
	free(pre);
	free(visited);
	return result;
}

int main() {
	int t, n;
	vector<vertex> * graph = new vector<vertex>[MAX_N];
	scanf("%d", &t);
	while (t) {
		scanf("%d", &n);
		int w;
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				scanf("%d", &w);
				if (w) graph[i].push_back(j);
			}
		}
		if (hungarian(graph, n)) printf("Yes\n");
		else printf("No\n");
		for (size_t i = 0; i < n; i++)
			graph[i].clear();
		t--;
	}
	for (size_t i = 0; i < MAX_N; i++)
		vector<vertex>().swap(graph[i]);
	queue<vertex>().swap(vqueue);
	return 0;
}
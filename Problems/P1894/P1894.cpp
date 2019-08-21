#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char byte;
typedef byte vertex;
#define NOT_A_VERTEX 0xff

size_t hungarian(vector<vertex> * graph, size_t left_num, size_t right_num) {
	vertex * left_matching = new vertex[left_num];
	fill(left_matching, left_matching + left_num, NOT_A_VERTEX);
	vertex * right_matching = new vertex[right_num];
	fill(right_matching, right_matching + right_num, NOT_A_VERTEX);
	vertex * pre = new vertex[left_num];
	fill(pre, pre + left_num, NOT_A_VERTEX);
	bool * visited = new bool[right_num];
	size_t result = 0;
	queue<vertex> vqueue;
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
		} //while
	NEXT:
		if (left_matching[v] != NOT_A_VERTEX) result++;
	} //for (vertex v = 0; v < left_num; v++)
	free(left_matching);
	free(right_matching);
	free(pre);
	free(visited);
	queue<vertex>().swap(vqueue);
	return result;
}

void readByte(byte * p_byte) {
	int i;
	scanf("%d", &i);
	*p_byte = i;
}

int main() {
	byte n, m, s, v;
	readByte(&n);
	readByte(&m);
	vector<vertex> * graph = new vector<vertex>[n];
	size_t i, j;
	for (i = 0; i < n; i++) {
		readByte(&s);
		for (j = 0; j < s; j++) {
			readByte(&v);
			v--;
			graph[i].push_back(v);
		}
	}
	printf("%d", hungarian(graph, n, m));
	for (i = 0; i < n; i++)
		vector<vertex>().swap(graph[i]);
	return 0;
}
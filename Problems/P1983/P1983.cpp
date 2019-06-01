#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short word;

#define WORD_MIN 0x0000
#define WORD_MAX 0xffff

class TopSort {
public:
	word getMaxDist(vector<word> * graph, size_t nv, word * in_degree);

private:
	word * level_;
	queue<word> vqueue_;
};

word TopSort::getMaxDist(vector<word>* graph, size_t nv, word * in_degree) {
	level_ = new word[nv];
	memset(level_, 0, nv * sizeof(word));
	for (size_t i = 0; i < nv; i++) 
		if (!in_degree[i]) {
			vqueue_.push(i);
		}
	word  max_dist = 0;
	word front_v, adj_v;
	while (!vqueue_.empty()) {
		front_v = vqueue_.front();
		vqueue_.pop();
		if (level_[front_v] > max_dist) max_dist = level_[front_v];
		for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) {
			adj_v = *it;
			if (level_[front_v] + 1 > level_[adj_v]) {
				level_[adj_v] = level_[front_v] + 1;
			}
			if ((--in_degree[adj_v]) == 0) {
				vqueue_.push(adj_v);
			}
		}
	}
	free(level_);
	return max_dist;
}

int main() {
	word n, m;
	scanf("%hd %hd", &n, &m);
	vector<word> * graph = new vector<word>[n];
	word * in_degree = new word[n];
	memset(in_degree, 0, n * sizeof(word));
	bool * stop = new bool[n];	
	vector<word> stop_stations;
	bool ** has_edge = new bool*[n];
	for (size_t i = 0; i < n; i++) {
		has_edge[i] = new bool[n];
		memset(has_edge[i], false, n * sizeof(bool));
	}
	word s, station, max_station, min_station;
	for (size_t i = 0, j; i < m; i++) {
		memset(stop, false, n * sizeof(bool));
		stop_stations.clear();
		scanf("%hd", &s);
		max_station = WORD_MIN;
		min_station = WORD_MAX;
		for (j = 0; j < s; j++) {
			scanf("%hd", &station);
			station--;
			stop[station] = true;
			stop_stations.push_back(station);
			if (station > max_station) max_station = station;
			if (station < min_station) min_station = station;
		}
		for (j = min_station + 1; j < max_station; j++) {
			if (!stop[j]) {
				for (word stop_station : stop_stations) {
					if (has_edge[j][stop_station]) continue;
					graph[j].push_back(stop_station);
					in_degree[stop_station]++;
					has_edge[j][stop_station] = true;
				}
			}
		}
	}
	free(stop);
	vector<word>().swap(stop_stations);
	for (size_t i = 0; i < n; i++) {
		free(has_edge[i]);
	}
	free(has_edge);
	TopSort topsort;
	word max_dist = topsort.getMaxDist(graph, n, in_degree);
	printf("%hd", max_dist + 1);
	for (size_t i = 0; i < n; i++)
		vector<word>().swap(graph[i]);
	free(in_degree);
	return 0;
}
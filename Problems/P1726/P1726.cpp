#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

typedef unsigned short word;

class Tarjan {
public:
	void tarjan(vector<word> * graph, word nv, vector<word>& max_scc);
private:
	void dfs(vector<word> * graph, word vertex, vector<word>& max_scc);

	word count_;
	word * dfn_;
	word * low_;
	bool * is_in_stack_;
	stack<word> vstack_;
};

void Tarjan::tarjan(vector<word>* graph, word nv, vector<word>& max_scc) {
	count_ = 0;
	dfn_ = new word[nv];
	fill(dfn_, dfn_ + nv, 0);
	low_ = new word[nv];
	is_in_stack_ = new bool[nv];
	fill(is_in_stack_, is_in_stack_ + nv, false);
	for (word v = 0; v < nv; v++) {
		dfs(graph, v, max_scc);
	}
	free(dfn_);
	free(low_);
	free(is_in_stack_);
}

void Tarjan::dfs(vector<word>* graph, word vertex, vector<word>& max_scc) {
	dfn_[vertex] = ++count_;
	low_[vertex] = dfn_[vertex];
	vstack_.push(vertex);
	is_in_stack_[vertex] = true;
	word adj_v;
	for (auto it = graph[vertex].begin(); it != graph[vertex].end(); it++) {	
		adj_v = *it;
		if (!dfn_[adj_v]) {
			dfs(graph, adj_v, max_scc);
			if (low_[adj_v] < low_[vertex]) low_[vertex] = low_[adj_v];
		}
		else if (is_in_stack_[adj_v]) {
			if (dfn_[adj_v] < low_[vertex]) low_[vertex] = dfn_[adj_v];
		}
	}
	if (dfn_[vertex] == low_[vertex]) {
		vector<word> scc;
		while (vstack_.top() != vertex) {
			scc.push_back(vstack_.top());
			is_in_stack_[vstack_.top()] = false;
			vstack_.pop();
		}
		is_in_stack_[vertex] = false;
		scc.push_back(vstack_.top());
		vstack_.pop();
		bool flag = scc.size() > max_scc.size();
		bool has_sorted = false;
		if (scc.size() == max_scc.size()) {
			sort(scc.begin(), scc.end());
			has_sorted = true;
			for (size_t i = 0; i < scc.size(); i++) {
				if (scc[i] > max_scc[i]) break;
				if (scc[i] < max_scc[i]) {
					flag = true;
					break;
				}
			}
		}
		if (flag) {
			max_scc.clear();
			max_scc.resize(scc.size());
			for (size_t i = 0; i < scc.size(); i++)
				max_scc[i] = scc[i];
			if (!has_sorted) sort(max_scc.begin(), max_scc.end());
		}
	}
}

int main() {
	word n, m;
	scanf("%hd %hd", &n, &m);
	vector<word> * graph = new vector<word>[n];
	word a, b, t;
	for (size_t i = 0; i < m; i++) {
		scanf("%hd %hd %hd", &a, &b, &t);
		a--; b--;
		graph[a].push_back(b);
		if (t == 2) graph[b].push_back(a);
	}
	Tarjan tarjan;
	vector<word> max_scc;
	tarjan.tarjan(graph, n, max_scc);
	printf("%d\n", max_scc.size());
	for (size_t i = 0; i < max_scc.size(); i++) {
		if (i != 0) putchar(' ');
		printf("%hd", max_scc[i] + 1);
	}
	for (size_t i = 0; i < n; i++)
		vector<word>().swap(graph[i]);
	return 0;
}
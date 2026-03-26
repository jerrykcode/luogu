#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <iostream>
const int V = 55;
enum Tag {
	R,P,X,NONE
};
struct SetElement {
	int u; // graph vertex
	Tag tag; // belongs to R/P/X, or none of them
};
int g[V][V];
int max_clique_size;
void bk(std::vector<SetElement>& set) {
	int pivot, pivot_score = -1;
	// find pivot
	for (SetElement& se : set) {
		if (se.tag == X || se.tag == P) {
			int score = 0;
			for (SetElement& pse : set) {
				if (pse.tag == P) {
					if (g[se.u][pse.u]) score++;
				}
			}
			if (score > pivot_score) {
				pivot_score = score;
				pivot = se.u;
			}
		}
	}
	bool pIsEmpty = true;
	for (SetElement& se : set) {
		if (se.tag == P && !g[pivot][se.u]) {
			pIsEmpty = false;
			// add se to R
			se.tag = R;
			std::vector<SetElement> new_set = set;
			for (SetElement& nse : new_set)
				if (nse.u != se.u)
					if (nse.tag == P || nse.tag == X)
						if (!g[se.u][nse.u]) nse.tag = NONE;
			bk(new_set);
			// set unchanged
			// move to X
			se.tag = X;
		}
	}
	if (pIsEmpty) { // P empty
		int size = 0;
		bool xIsEmpty = true;
		for (SetElement& se : set) {
			if (se.tag == X) {
				xIsEmpty = false; break; // -1 always < max_clique_sizes
			} else if (se.tag == R) {
				size++;
			}
		}
		if (xIsEmpty && size > max_clique_size) max_clique_size = size;
		return;
	}
	
}
int _g[V][V];
void degeneracy(int n) {
	std::vector<int> order(n);
	std::set<int> used;
	memcpy(_g, g, sizeof(g));
	for (int i = 0; i < n; i++) {
		int min_d = n + 1, min_d_u;
		for (int u = 0; u < n; u++) {
			if (used.find(u) != used.end()) continue;
			int d = 0;
			for (int v = 0; v < n; v++)
				if (_g[u][v]) d++;
			if (d < min_d) {
				min_d = d;
				min_d_u = u;
			}
		}
		order[i] = min_d_u;
		used.insert(min_d_u);
		for (int v = 0; v < n; v++)
			if (_g[min_d_u][v]) {
				_g[min_d_u][v] = _g[v][min_d_u] = 0;
			}
	}
	std::vector<SetElement> set(n);
	for (int i = 0; i < n; i++) {
		int u = order[i];
		for (int j = 0; j < i; j++) {
			set[j].u = order[j];
			if (g[order[j]][u]) set[j].tag = X; else set[j].tag = NONE;
		}
		set[i].u = u;
		set[i].tag = R;
		for (int j = i + 1; j < n; j++) {
			set[j].u = order[j];
			if (g[order[j]][u]) set[j].tag = P; else set[j].tag = NONE;
		}
		bk(set);
	}
}
int main() {
	memset(g, 0, sizeof(g));
	int n, a, b;
	std::cin>>n;
	while (std::cin>>a) {
		std::cin>>b;
		a--, b--;
		g[a][b] = g[b][a] = 1;
	}
	max_clique_size = 0; // init
	bool need_degeneracy = n > 3;
	if (!need_degeneracy) {
	std::vector<SetElement> set(n);
		for (int i = 0; i < n; i++) {
			set[i].u = i;
			set[i].tag = P;
		}
		bk(set);
	} else {
		degeneracy(n);
	}
	std::cout<<max_clique_size<<std::endl;
	exit(0);
}
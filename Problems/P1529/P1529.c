#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "memory.h"

typedef unsigned short word;
#define WORD_MAX 0xffff
#define MAX_V 52

word ** graph;
word * dist;
bool * collected;

size_t get_index(char c) {
	if (c >= 'a' && c <= 'z') {
		return (size_t)c - (size_t)'a';
	}
	else if (c >= 'A' && c <= 'Z') {
		return (size_t)c - (size_t)'A' + 26;
	}
	return 0xffffffff; //Error
}

void dijkstra(char src) {	
	memset(dist, WORD_MAX, MAX_V * sizeof(word));
	memset(collected, false, MAX_V * sizeof(bool));
	size_t src_idx = get_index(src);
	dist[src_idx] = 0;
	while (1) {
		size_t min_v_idx = MAX_V;
		for (size_t idx = 0; idx < MAX_V; idx++) 
			if (!collected[idx] && (min_v_idx == MAX_V || dist[idx] < dist[min_v_idx])) {
				min_v_idx = idx;
			}
		if (min_v_idx == MAX_V) break;
		collected[min_v_idx] = true;
		for (size_t adj_v_idx = 0; adj_v_idx < MAX_V; adj_v_idx++)
			if (!collected[adj_v_idx] && graph[min_v_idx][adj_v_idx] != WORD_MAX)
				if (dist[min_v_idx] + graph[min_v_idx][adj_v_idx] < dist[adj_v_idx] || dist[adj_v_idx] == WORD_MAX) {
					dist[adj_v_idx] = dist[min_v_idx] + graph[min_v_idx][adj_v_idx];
				}			
	}
}

int main() {
	graph = (word **)malloc(MAX_V * sizeof(word *));
	for (size_t i = 0; i < MAX_V; i++) {
		graph[i] = (word *)malloc(MAX_V * sizeof(word));
		memset(graph[i], WORD_MAX, MAX_V * sizeof(word));
	}
	dist = (word *)malloc(MAX_V * sizeof(word));	
	collected = (bool *)malloc(MAX_V * sizeof(bool));	
	int p;
	scanf("%d", &p);
	char a, b;
	word len;
	size_t a_idx, b_idx;
	for (size_t i = 0; i < p; i++) {
		scanf(" %c %c %hd", &a, &b, &len);
		if (a == b) continue;
		a_idx = get_index(a);
		b_idx = get_index(b);	
		if (len < graph[a_idx][b_idx]) {
			graph[a_idx][b_idx] = len;
			graph[b_idx][a_idx] = len;
		}
	}
	dijkstra('Z');
	char min_char = 'A';
	for (char c = 'B'; c < 'Z'; c++) //'B' ~ 'Y'
		if (dist[get_index(c)] < dist[get_index(min_char)]) {
			min_char = c;
		}
	printf("%c %hd", min_char, dist[get_index(min_char)]);
	for (size_t i = 0; i < MAX_V; i++)
		free(graph[i]);
	free(graph);
	free(dist);
	free(collected);
	return 0;
}
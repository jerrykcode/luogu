#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char byte; //n <= 50，一个字节足够
typedef byte vertex; //顶点，左图表示在学校住的人，右图表示床位
#define BYTE_MAX 0xff
#define NOT_A_VERTEX BYTE_MAX //表示不存在的顶点

struct Person {
	bool is_student; //true则为在校生
	bool go_home; //true则要回家，对于非在校生此项无效
	vector<vertex> list; //可以睡的床位
	~Person() {
		vector<vertex>().swap(list);
	}
};

queue<vertex> q; //顶点队列
bool hungarian(Person * p, int n) {
	vertex * left_matching = new vertex[n]; //left_matching[v]表示左边顶点v匹配到的右边顶点
	fill(left_matching, left_matching + n, NOT_A_VERTEX);
	vertex * pre = new vertex[n]; //pre[v]表示若v找到新的匹配，则v原来的匹配可以给pre[v]
	fill(pre, pre + n, NOT_A_VERTEX);
	vertex * right_matching = new vertex[n]; //right_matching[v]表示右边顶点v匹配到的左边顶点
	fill(right_matching, right_matching + n, NOT_A_VERTEX);
	bool * visited = new bool[n]; //visited[v]表示BFS中右边顶点v是否访问过
	bool result = true; //结果，初始化为true
	for (vertex v = 0; v < n; v++) { //遍历所有人
		if (p[v].is_student && p[v].go_home) continue; //若v是在校生且回家，则他不用匹配床位
		//否则v需要匹配一个床位，v是左边顶点
		fill(visited, visited + n, false);
		q.push(v);
		vertex front_v, adj_v, from, to, tmp_v;
		while (!q.empty()) {
			front_v = q.front();
			q.pop();
			for (auto it = p[front_v].list.begin(); it != p[front_v].list.end(); it++) {
				adj_v = *it; //front_v的邻接点，(右边顶点)
				if (!visited[adj_v]) {
					visited[adj_v] = true;
					if (right_matching[adj_v] != NOT_A_VERTEX) { //adj_v有匹配的左边顶点，即这张床有人匹配
						q.push(right_matching[adj_v]); //这张床匹配的人进队列，相当于DFS中的递归
						pre[right_matching[adj_v]] = front_v; //若以后这张床匹配的人找到了新的床匹配，那么原来这张床可以给front_v匹配，pre数组起到了记录的作用
					}
					else { //adj_v没有匹配的左边顶点，即这张床还没有人匹配，即front_v找到床位了
						from = front_v;
						to = adj_v;
						while (from != NOT_A_VERTEX) {
							tmp_v = left_matching[from]; //tmp_v为from原来的匹配，若from原来没有匹配，则tmp_v为NOT_A_VERTEX
							left_matching[from] = to; //左边顶点from匹配到的右边顶点更新为to
							right_matching[to] = from; //右边顶点to匹配到的左边顶点更新为from
							 //from找到匹配了，向前迭代，之前匹配的tmp_v给pre[from]
							from = pre[from];
							to = tmp_v; //from原来的匹配
						}
						while (!q.empty()) q.pop();
						goto NEXT;
					}
				}
			} //for auto it
		} //while (!q.empty())
	NEXT:
		if (left_matching[v] != NOT_A_VERTEX) continue; //v找到了匹配，继续
		else {
			//v无法匹配到床位
			result = false;
			break;
		}
	} //for v
	free(left_matching);
	free(pre);
	free(right_matching);
	free(visited);
	return result;
}

//输入一字节
void inputByte(byte* p_byte) {
	int in;
	scanf("%d", &in);
	*p_byte = in;
}

void inputBoolean(bool * p_bool) {
	int in;
	scanf("%d", &in);
	if (in) *p_bool = true;
	else *p_bool = false;
}

int main() {
	byte t, n, i, j, k;
	inputByte(&t);
	for (i = 0; i < t; i++) {
		inputByte(&n);
		Person * p = new Person[n];
		for (j = 0; j < n; j++) inputBoolean(&p[j].is_student);
		for (j = 0; j < n; j++) inputBoolean(&p[j].go_home);
		for (j = 0; j < n; j++) {
			if (p[j].is_student && !p[j].go_home) p[j].list.push_back(j); //若在校且不回家，可以睡自己的床
			bool know_each_other;
			for (k = 0; k < n; k++) {
				inputBoolean(&know_each_other);
				if (know_each_other && !(p[j].is_student && p[j].go_home) && p[k].is_student) { //j认识k且j在学校住且k是在校生，j可以睡k的床
					p[j].list.push_back(k);
				}
			} //for k
		} //for j
		hungarian(p, n) ? printf("^_^\n") : printf("T_T\n");
		for (j = 0; j < n; j++) {
			p[j].~Person();
		}
	} //for i	
	return 0;
}
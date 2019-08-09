# P2055 [ZJOI2009]假期的宿舍
[原题地址](https://www.luogu.org/problem/P2055)

## 题目描述
学校放假了...... 有些同学回家了，而有些同学则有以前的好朋友来探访，那么住宿就是一个问题。

比如 A 和 B 都是学校的学生，A 要回家，而 C 来看B，C 与 A 不认识。我们假设每个人只能睡和自己直接认识的人的床。那么一个解决方案就是 B 睡 A 的床而 C 睡 B 的床。而实际情况可能非常复杂，有的人可能认识好多在校学生，在校学生之间也不一定都互相认识。

我们已知一共有 n 个人，并且知道其中每个人是不是本校学生，也知道每个本校学生是否回家。问是否存在一个方案使得所有不回家的本校学生和来看他们的其他人都有地方住。

## 输入格式

第一行一个数 T 表示数据组数。接下来 T 组数据，每组数据第一行一个数n 表示涉及到的总人数。

接下来一行 n 个数，第 i 个数表示第 i 个人是否是在校学生 (0 表示不是，1 表示是)。再接下来一行 n 个数，第 i 个数表示第 i 个人是否回家 (0 表示不回家，1 表示回家，注意如果第 i 个人不是在校学生，那么这个位置上的数是一个随机的数，你应该在读入以后忽略它)。

接下来 n 行每行 n 个数，第 i 行第 j 个数表示 i 和 j 是否认识 (1 表示认识，0 表示不认识，第 i 行 i 个的值为 0，但是显然自己还是可以睡自己的床)，认识的关系是相互的。

## 输出格式

对于每组数据，如果存在一个方案则输出 “^_^”(不含引号) 否则输出“T_T”(不含引号)。(注意输出的都是半角字符，即三个符号的 ASCII 码分别为94,84,95)

## 输入输出样例

### 输入 #1

> 1  
> 3  
> 1 1 0  
> 0 1 0  
> 0 1 1  
> 1 0 0  
> 1 0 0

### 输出 #1

> ^_^

## 说明/提示

对于 30% 的数据满足 1 <= n <= 12。

对于 100% 的数据满足 1 <= n <= 50,1 <= T <= 20。

`多组数据！！！`

## 思路
二分图最大匹配(这里使用匈牙利算法BFS实现)，二分图模板见[`P3386二分图匹配`题解](https://github.com/jerrykcode/luogu/blob/master/Problems/P3386/Solution.md)

要在学校住的人为左图，床位为右图。

使用结构体
```cpp
struct Person {
	bool is_student;
	bool go_home;
	vector<vertex> list;
	~Person() {
		vector<vertex>().swap(list);
	}
};
```
记录一个人是否在校，是否回家，以及他可以睡的床位，`(is_student && go_home)`表示这个人是在校生且要回家，那么他是不在学校住的，其余的`!(is_student && go_home)`就要在学校住。

对于每组数据，用Person结构体数组存储每个人，首先输入每个人是否在校，是否回家。然后输入认识关系。假设a认识b，那么若a要在学校住且b是在校生，则a可以睡b的床。注意不回家的在校生是可以睡他自己的床的。
若一个人可以睡某张床，则此人(左图顶点)和该床(右图顶点)之间连边。人的编号为`0 ~ n - 1`(实际在学校住的人数小于等于n，对于回家的在校生，忽略他们即可)，床的编号`0 ~ n - 1`(实际上床数等于在校生的个数，故0 ~ n - 1不一定每个数字都有相应编号的床，但这并不影响)。

对二分图求最大匹配即可。

## 代码

### C++


```cpp
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
```

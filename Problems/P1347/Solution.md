# P1347 排序
[原题地址](https://www.luogu.org/problemnew/show/P1347)

## 题目描述

一个不同的值的升序排序数列指的是一个从左到右元素依次增大的序列，例如，一个有序的数列A,B,C,D 表示A<B,B<C,C<D。在这道题中，我们将给你一系列形如A<B的关系，并要求你判断是否能够根据这些关系确定这个数列的顺序。

## 输入输出格式

### 输入格式：

第一行有两个整数n,m，n表示需要排序的元素数量，2<=n<=26，第1到n个元素将用大写的A,B,C,D....表示。m表示将给出的形如A<B的关系的数量。

接下来有m行，每行有3个字符，分别为一个大写字母，一个<符号，一个大写字母，表示两个元素之间的关系。

### 输出格式：

若根据前x个关系即可确定这n个元素的顺序yyy..y（如ABC），输出

> Sorted sequence determined after xxx relations: yyy...y.

若根据前x个关系即发现存在矛盾（如A<B,B<C,C<A），输出

> Inconsistency found after 2 relations.

若根据这m个关系无法确定这n个元素的顺序，输出

> Sorted sequence cannot be determined.

（提示：确定n个元素的顺序后即可结束程序，可以`不用考虑确定顺序之后出现矛盾的情况`）

## 输入输出样例

### 输入样例#1： 
> 4 6  
> A<B  
> A<C  
> B<C  
> C<D  
> B<D  
> A<B  

### 输出样例#1： 

> Sorted sequence determined after 4 relations: ABCD.


### 输入样例#2： 
> 3 2  
> A<B  
> B<A  

### 输出样例#2： 

> Inconsistency found after 2 relations.

### 输入样例#3： 
> 26 1  
> A<Z  

## 输出样例#3： 

> Sorted sequence cannot be determined.

## 思路

拓扑排序。x<y则x到y有一条有向边。建立图。

每插入一条边，即进行拓扑排序，此时图的顶点是已输入的边中的顶点（不一定所有顶点都输入过了）。记录进队列的顶点数目，记录最大层数。若记录的顶点数目小于图的顶点数目，则有环。否则无环。若最大层数等于图的顶点数量，则已完成排序，否则无法确定排序。在发现有环时即结束输入，在所有顶点都在边中被输入后，若完成排序则也结束输入，否则继续输入并插入边。每次拓扑排序过程中用vector存储入队列的顶点，在所有顶点都在边中被输入且完成排序后输出。

## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

#define MAX_LETTER 26
#define NO_VERTEX 0xff

typedef unsigned char BYTE;
typedef BYTE Vertex; //最多只有26个顶点，unsigned char足够

//拓扑排序类
class Topsort {
public:
    Topsort() {}
    ~Topsort() {
        vector<Vertex>().swap(order);
    }

    //拓扑排序结果
    typedef enum {
        SORTED, //确定排序
        UNSORTED, //无法确定排序
        HAS_LOOP, //有环
    } TopRESULT;

    /* 拓扑排序，返回结果。*/
    Topsort::TopRESULT top(vector<Vertex> *graph, int nv, int *indegree);

    /* 输出排序，vertex_to_letter是顶点序号对应的字母。*/
    void print_order(char *vertex_to_letter);

private:
    //类变量
    vector<Vertex> order; //拓扑序列
    queue<Vertex> q;
};

Topsort::TopRESULT Topsort::top(vector<Vertex>* graph, int nv, int * indegree) {
    order.clear();
    int *level = new int[nv]; //层数
    int vcount = 0;
    for (int i = 0; i < nv; i++) 
        if (indegree[i] == 0) {
            q.push((Vertex)i);
            order.push_back((Vertex)i);
            level[i] = 1;
            vcount++;
        }
    int max_level = 1; //最大层数
    while (!q.empty()) {
        Vertex v = q.front();
        q.pop();
        for (Vertex w : graph[(int)v]) {
            if (--indegree[w] == 0) {
                q.push(w);
                order.push_back(w);
                level[(int)w] = level[(int)v] + 1;
                if (level[(int)w] > max_level) max_level = level[(int)w];
                vcount++;
            }
        }
    }
    free(level);
    if (vcount == nv) {
        return max_level == nv ? SORTED : UNSORTED;
    }
    else return HAS_LOOP;
}

void Topsort::print_order(char * vertex_to_letter) {
    for (auto it = order.begin(); it != order.end(); it++) {
        printf("%c", vertex_to_letter[(int)(*it)]);
    }
}

int main() {
    int n, m, a, b;
    scanf("%d %d", &n, &m);
    vector<Vertex> *graph = new vector<Vertex>[n]; //图
    int *indegree = new int[n]; //入度数组
    fill(indegree, indegree + n, 0);
    int *indegree_copy = new int[n]; //入度数组拷贝
    Vertex *letter_to_vertex = new Vertex[MAX_LETTER]; //字母对应顶点序号
    fill(letter_to_vertex, letter_to_vertex + MAX_LETTER, NO_VERTEX); 
    char *vertex_to_letter = new char[n]; //顶点序号对应字母
    int count = 0;
    char *s = new char[3];
    Topsort topsort;
    bool unsorted_flag = true;
    for (int i = 0; i < m; i++) {
        scanf("%s", s);
        a = s[0] - 'A';
        b = s[2] - 'A';
        if (letter_to_vertex[a] == NO_VERTEX) {
            //标记
            letter_to_vertex[a] = (Vertex)count;
            vertex_to_letter[count++] = s[0];
        }
        if (letter_to_vertex[b] == NO_VERTEX) {
            //标记
            letter_to_vertex[b] = (Vertex)count;
            vertex_to_letter[count++] = s[2];
        }
        graph[letter_to_vertex[a]].push_back(letter_to_vertex[b]);
        indegree[letter_to_vertex[b]]++;
        memcpy(indegree_copy, indegree, n * sizeof(int)); //复制入度数组
        Topsort::TopRESULT top_result = topsort.top(graph, count, indegree_copy); //注意传入count(当前已输入的顶点数)不是n
        switch (top_result) {
        case Topsort::SORTED: {
            if (n == count) { //未输入所有顶点也不能看作排序成功
                printf("Sorted sequence determined after %d relations: ", i + 1);
                topsort.print_order(vertex_to_letter);
                printf(".");
                unsorted_flag = false;
                goto END;
            }
            break;
        }
        case Topsort::UNSORTED: {
            break;
        }
        case Topsort::HAS_LOOP: { //有环
            printf("Inconsistency found after %d relations.", i + 1);
            unsorted_flag = false;
            goto END;
        }
        }
    }
END:
    if (unsorted_flag) {
        printf("Sorted sequence cannot be determined.");
    }
    for (int i = 0; i < n; i++)
        vector<Vertex>().swap(graph[i]);
    free(indegree);
    free(indegree_copy);
    free(letter_to_vertex);
    free(vertex_to_letter);
    return 0;    
}

```
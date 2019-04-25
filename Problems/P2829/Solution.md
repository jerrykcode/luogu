# P2829 ������
[ԭ���ַ](https://www.luogu.org/problemnew/show/P2829)

��Ŀ����
----

zrz�߽���һ��������Թ����������Լ���·�ˣ����ӳ��������ò��������������е�·���۵Ŀ����ˣ�ֻ�ý����æ����

��Ŀ����
----

����һ����n���ڵ��ͼ����m��˫��ߣ�ÿһ��·��w����λ���룬zrz��1��λ�ã�������n��λ�ã�����zrz���ӳ��˵�bug�����ǲ�������̵�·�����ߵ�2�̵�·����2��·�����������·�����رߣ�Ȼ��Ҳ�����ظ�ͨ��һЩ�ڵ��·��ע������ж���·���������·������ô���Ƕ����ܽе�2��·��������zrz������������������һ���ڵ���ֱ�����ӵĵط�С��k���������յ���⣩����ô���Ͳ��ҽ�ȥ��

���������ʽ
------

�����ʽ��

��һ��3������n,m,k

������m�У�ÿ��3������u��v��w����ʾ��u��v��һ��ȨֵΪw�ıߡ���u,v<=n��w<=10000��

�����ʽ��

һ��������ʾ��s�ߵ�t�ĵ�2��·��ֵ����������ڣ����-1

�����������
------

### ��������#1��

> 4 4 1  
> 1 2 100  
> 2 4 200  
> 2 3 250  
> 3 4 100  

### �������#1��

> 450  

### ��������#2��

> 4 4 3  
> 1 2 100  
> 2 4 200  
> 2 3 250  
> 3 4 100  

### �������#2��

500

˵��
--

����50%�����ݣ�n<=10,m<=10

����90%�����ݣ�n<=1000,m<=20000

����100%�����ݣ�n<=5000��m<=100000

���⣬k�Ƚ�С

����2���·����300��1-2-4������Ϊ��2�޷��ߵ�3��3���ӵ��Ľڵ�ֻ��2���������Կ���1-2-1-2-4���ڶ���·Ϊ500��

## ˼·

A*��ζ�·������Dijkstra�������ÿ�����㵽�յ����̾���Ϊ���ۺ�����

## ����

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned short Vertex; //����
#define NO_VALUE -1

//�ڽӵ�ṹ��
struct AdjNode {
    Vertex adj_v; //�ڽӵ�
    short adj_weight; //�ڽӱ�Ȩ��
    AdjNode(Vertex adj_v, short adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
};

//��������ͼ�ζ�·����
class SecondShortestPath {
public:
    /* ��������ͼ�ζ�·��
        @param graph ͼ
        @param nv ������
        @param src Դ
        @param des �յ�
        @param num ���飬�洢ÿ��������ڽӵ�����graph�������رߣ�
        @param k ��չ�ڽӵ�ʱ�����ڽӵ㲻��src��des�������ڽӵ���С��k������չ���ڽӵ�
        @param p_second_shortest_dist ָ�룬�����ζ�·����
        @return bool ���ζ�·�����򷵻�true
    */
    bool getSecondShortestPath(vector<AdjNode> *graph, int nv, int src, int des, int *num, int k, int *p_second_shortest_dist);
private:
    /* Dijkstra�������·��
        @param graph ͼ
        @param nv ������
        @param src Դ
        @param des �յ�
        @param num ���飬�洢ÿ��������ڽӵ�����graph�������رߣ�
        @param k ��չ�ڽӵ�ʱ�����ڽӵ㲻��src��des�������ڽӵ���С��k������չ���ڽӵ�
        @param dist ���飬����src��ÿһ���������̾���
        @return bool ��src�ɴ�des�򷵻�true
    */
    bool dijkstra(vector<AdjNode> *graph, int nv, int src, int des, int *num, int k, int *dist);

    /* A*����ζ�·��
        @param graph ͼ
        @param h ���� A*�㷨�Ĺ�ֵ����
        @param nv ������
        @param src Դ
        @param des �յ�
        @param num ���飬�洢ÿ��������ڽӵ�����graph�������رߣ�
        @param k ��չ�ڽӵ�ʱ�����ڽӵ㲻��src��des�������ڽӵ���С��k������չ���ڽӵ�
        @param p_second_shortest_dist ָ�룬�����ζ�·����
        @return bool ���ζ�·�����򷵻�true
    */
    bool astar(vector<AdjNode> *graph, int *h, int nv, int src, int des, int *num, int k, int *p_second_shortest_dist);

    //Dijkstra���ȶ���ʹ�õĽṹ��
    struct PriorityNode {
        Vertex v;
        int dist;
        PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
    };

    //A*���ȶ���ʹ�õĽṹ�壬�̳���PriorityNode
    struct AstarNode : public PriorityNode {
        //f = dist
        int g;
        AstarNode(Vertex v, int g, int f) : PriorityNode(v, f), g(g) {}
    };

    //���ȶ������ڱȽϵĽṹ��
    struct cmp {
        bool operator () (PriorityNode& a, PriorityNode& b) {
            return a.dist > b.dist;
        }
    };
};

bool SecondShortestPath::getSecondShortestPath(vector<AdjNode>* graph, int nv, int src, int des, int *num, int k, int *p_second_shortest_dist) {
    int *h = new int[nv];
    bool result;
    if (dijkstra(graph, nv, des, src, num, k, h)) { //����������·����ΪA*�Ĺ�ֵ����
        result = astar(graph, h, nv, src, des, num, k, p_second_shortest_dist); //A*
    }
    else result = false;
    free(h);
    return result;
}

bool SecondShortestPath::dijkstra(vector<AdjNode>* graph, int nv, int src, int des, int *num, int k, int * dist) {
    fill(dist, dist + nv, NO_VALUE);
    bool *collected = new bool[nv];
    fill(collected, collected + nv, false);
    dist[src] = 0;
    priority_queue<PriorityNode, vector<PriorityNode>, cmp> q;
    q.push(PriorityNode(src, 0));
    Vertex top_v, adj_v;
    int tmp_dist;
    while (!q.empty()) {
        top_v = q.top().v;
        q.pop();
        if (collected[top_v]) continue;
        collected[top_v] = true;
        for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //�����ڽӵ�
            adj_v = it->adj_v;
            if ((num[adj_v] >= k || adj_v == des) && !collected[adj_v]) { //�ڽӵ���ڽӵ���Ŀ��Ҫ>=k�������ڽӵ����des
                tmp_dist = dist[top_v] + (int)it->adj_weight;
                if (tmp_dist < dist[adj_v] || dist[adj_v] == NO_VALUE) {
                    dist[adj_v] = tmp_dist;
                    q.push(PriorityNode(adj_v, tmp_dist));
                }
            }
        } //for
    } //while
    free(collected);
    return dist[des] != NO_VALUE;
}

bool SecondShortestPath::astar(vector<AdjNode>* graph, int *h, int nv, int src, int des, int *num, int k, int * p_second_shortest_dist) {
    priority_queue<AstarNode, vector<AstarNode>, cmp> q;
    q.push(AstarNode(src, 0, h[src]));
    Vertex top_v, adj_v;
    int top_g, last_g = NO_VALUE, tmp_g, count = 0;
    while (!q.empty()) {
        top_v = q.top().v;
        top_g = q.top().g;
        q.pop();
        if (top_v == des && top_g != last_g) { //����des����des��gֵ���ϴε���des��gֵ��ͬ
            if (++count == 2) { //����
                while (!q.empty()) q.pop();
                *p_second_shortest_dist = top_g;
                return true;
            }
            last_g = top_g; //����last_gΪ��ε���des��gֵ
        }
        for (auto it = graph[top_v].begin(); it != graph[top_v].end(); it++) { //�����ڽӵ�
            adj_v = it->adj_v;
            if (num[adj_v] >= k || adj_v == src || adj_v == des) { //�ڽӵ���Ҫ>=k�����ڽӵ����src����des
                tmp_g = top_g + (int)it->adj_weight;
                q.push(AstarNode(adj_v, tmp_g, tmp_g + h[adj_v]));
            }
        }
    }
    return false;
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<AdjNode> *graph = new vector<AdjNode>[n];
    int *num = new int[n];
    fill(num, num + n, 0);
    for (int i = 0, u, v, w; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        u--; v--;
        graph[u].push_back(AdjNode(v, w));
        graph[v].push_back(AdjNode(u, w));
    }
    bool *appeared = new bool[n];	
    for (int i = 0; i < n; i++) {
        fill(appeared, appeared + n, false);
        for (auto it = graph[i].begin(); it != graph[i].end(); it++) {
            if (appeared[it->adj_v]) continue;
            else {
                num[i]++;
                appeared[it->adj_v] = true;
            }
        }
    }
    SecondShortestPath ssp;
    int second_shortest_dist;
    if (ssp.getSecondShortestPath(graph, n, 0, n - 1, num, k, &second_shortest_dist)) {
        printf("%d", second_shortest_dist);
    }
    else printf("-1");
    free(num);
    for (int i = 0; i < n; i++)
        vector<AdjNode>().swap(graph[i]);	
    return 0;
}


```
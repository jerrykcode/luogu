# P1550 [USACO08OCT]��Watering Hole
[ԭ���ַ](https://www.luogu.org/problemnew/show/P1550)

## ��Ŀ����

John��ũ��ȱˮ�ˣ�����

## ��Ŀ����

Farmer John has decided to bring water to his N (1 <= N <= 300) pastures which are conveniently numbered 1..N. He may bring water to a pasture either by building a well in that pasture or connecting the pasture via a pipe to another pasture which already has water.

Digging a well in pasture i costs W_i (1 <= W_i <= 100,000).

Connecting pastures i and j with a pipe costs P_ij (1 <= P_ij <= 100,000; P_ij = P_ji; P_ii=0).

Determine the minimum amount Farmer John will have to pay to water all of his pastures.

POINTS: 400

ũ��John ������ˮ���뵽����n(1<=n<=300)����������׼��ͨ������

�ɾ������ڸ�����������ˮ������ͨ��������Թ�ˮ���ڵ�i ��������һ�ھ���Ҫ����W_i(1<=W_i<=100,000)Ԫ������i ������j ������ҪP_ij (1 <= P_ij <= 100,000 , P_ji=P_ij)Ԫ��

�����ũ��John ��ҪΪʹ����ũ��������ˮ��ũ��������ӵ��ˮ������Ҫ��Ǯ����

## ���������ʽ

### �����ʽ��

* ��1 ��Ϊһ������n��

* ��2 ��n+1 ��ÿ��һ�����������ϵ��·ֱ�ΪW_1 ��W_n��

* ��n+2 ��2n+1 ��Ϊһ�����󣬱�ʾ��Ҫ�ľ��ѣ�P_ij����

### �����ʽ��

* ֻ��һ�У�Ϊһ����������ʾ����Ҫ��Ǯ����

## �����������

### ��������#1�� 

> 4  
> 5  
> 4  
> 4  
> 3  
> 0 2 2 2  
> 2 0 3 3  
> 2 3 0 4  
> 2 3 4 0  

### �������#1�� 

> 9  
## ˵��

John������ˮ����ֻ��1sʱ�䣡����

## ˼·

�����濴��һ�����㣬�����������������бߣ���ȨΪ�򾮷��á�Prim���ɡ�

## ����

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

#define NO_VALUE -1

typedef short Vertex; //����
//�ڽӵ�ṹ��
struct AdjNode {
    Vertex v; //�ڽӵ�
    int weight; //�ڽӱ�Ȩ��
    AdjNode(Vertex v, int weight) : v(v), weight(weight) {}
};

//Prim��
class Prim {
public:
    /* ������С��������Ȩ�ء�*/
    int prim(vector<AdjNode>* graph, int nv);
private:
    //���ȶ���ʹ�õĽṹ��
    struct PriorityNode {
        Vertex v;
        int dist;
        PriorityNode(Vertex v, int dist) : v(v), dist(dist) {}
    };

    struct cmp {
        bool operator() (PriorityNode& a, PriorityNode& b) {
            return a.dist > b.dist;
        }
    };

    //�����
    int *dist_;
    priority_queue < PriorityNode, vector<PriorityNode>, cmp> q;
};

int Prim::prim(vector<AdjNode>* graph, int nv) {
    //Primģ��
    dist_ = new int[nv];
    fill(dist_, dist_ + nv, NO_VALUE);
    Vertex src = 0;
    dist_[src] = 0;
    q.push(PriorityNode(src, 0));
    int mst_weight = 0; //�������ʼ��Ϊ0
    Vertex min_v, adj_v;
    int adj_weight;
    while (!q.empty()) {
        min_v = q.top().v;
        q.pop();
        if (min_v != src && dist_[min_v] == 0) continue;
        mst_weight += dist_[min_v];
        dist_[min_v] = 0;
        for (auto it = graph[min_v].begin(); it != graph[min_v].end(); it++) {
            //�����ڽӵ�
            adj_v = it->v;
            if (dist_[adj_v]) { //������MST��
                adj_weight = it->weight;
                if (adj_weight < dist_[adj_v] || dist_[adj_v] == NO_VALUE) {
                    //����
                    dist_[adj_v] = adj_weight;
                    q.push(PriorityNode(adj_v, adj_weight));
                }
            }
        }
    }//while
    free(dist_);
    return mst_weight;
}

int main() {
    int n;
    scanf("%d", &n);
    int nv = n + 1; //�ܶ�����
    vector<AdjNode> *graph = new vector<AdjNode>[nv];
    for (int i = 0, w; i < n; i++) {
        //����򾮷���
        scanf("%d", &w);	
        //�����(n�Ŷ���)����
        graph[i].push_back(AdjNode(n, w));
        graph[n].push_back(AdjNode(i, w));		
    }
    for (int i = 0, w; i < n; i++) {
        for (int j = 0; j < n; j++) {
            //������ͨ����
            scanf("%d", &w);
            if (i != j) {
                graph[i].push_back(AdjNode(j, w));
                graph[j].push_back(AdjNode(i, w));
            }
        }
    }
    Prim prim;
    printf("%d", prim.prim(graph, nv));
    for (int i = 0; i < nv; i++)
        vector<AdjNode>().swap(graph[i]);
    return 0;
}
```
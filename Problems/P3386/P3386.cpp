#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned int Vertex;
#define NO_VERTEX 0xffffffff

class Hungarian {
public:
    /*
    返回二分图最大匹配
    @param graph 图，只记录左边顶点到右边顶点的边
    @param left_num 左边顶点数量，左边顶点编号 0 ~ left_num - 1
    @param right_num  右边顶点数量，右边顶点编号 0 ~ right_num - 1
    @return size_t 最大匹配数量
    */
    virtual size_t maxMatching(vector<Vertex> * graph, size_t left_num, size_t right_num) = 0;

    virtual ~Hungarian() {
    
    }
};

//DFS实现
class DFS : public Hungarian {
public:
    virtual size_t maxMatching(vector<Vertex> * graph, size_t left_num, size_t right_num);
private:
    bool dfs(Vertex v);

    vector<Vertex> * graph_;
    Vertex * matching_;
    bool * visited_;
    size_t match_num_;
};

size_t DFS::maxMatching(vector<Vertex>* graph, size_t left_num, size_t right_num) {
    graph_ = graph;
    matching_ = new Vertex[right_num]; //matching_是右边顶点匹配的左边顶点，数组大小为right_num
    fill(matching_, matching_ + right_num, NO_VERTEX);
    visited_ = new bool[right_num]; //visited_标记右边顶点
    match_num_ = 0;
    for (size_t i = 0; i < left_num; i++) {
        fill(visited_, visited_ + right_num, false);
        if (dfs(i)) {
            match_num_++;
        }
    }
    free(matching_);
    free(visited_);
    return match_num_;
}

bool DFS::dfs(Vertex v) {
    Vertex adj_v;
    for (auto it = graph_[v].begin(); it != graph_[v].end(); it++) {
        adj_v = *it;
        if (!visited_[adj_v]) {
            visited_[adj_v] = true;
            if (matching_[adj_v] == NO_VERTEX || dfs(matching_[adj_v])) { //adj_v没有匹配或者adj_v的匹配可以找到其他匹配
                matching_[adj_v] = v;
                return true;
            }
        }
    }
    return false;
}

//BFS实现
class BFS : public Hungarian {
public:
    virtual size_t maxMatching(vector<Vertex> * graph, size_t left_num, size_t right_num);
private:

    Vertex * left_matching_;
    Vertex * right_matching_;

    queue<Vertex> vqueue_;
    Vertex * pre_;
    bool * visited_;
};

size_t BFS::maxMatching(vector<Vertex>* graph, size_t left_num, size_t right_num) {
    left_matching_ = new Vertex[left_num]; //左边顶点匹配的顶点
    fill(left_matching_, left_matching_ + left_num, NO_VERTEX);
    right_matching_ = new Vertex[right_num]; //右边顶点匹配的顶点
    fill(right_matching_, right_matching_ + right_num, NO_VERTEX);
    pre_ = new Vertex[left_num]; //BFS中左边顶点的前驱
    visited_ = new bool[right_num]; //右边顶点是否访问过
    size_t result = 0;
    for (Vertex i = 0; i < left_num; i++) { //遍历左边顶点
        fill(visited_, visited_ + right_num, false); //初始化右边所有顶点没有访问过		
        vqueue_.push(i);
        pre_[i] = NO_VERTEX; // i 没有前驱
        Vertex front_v, adj_v;
        while (!vqueue_.empty()) {
            front_v = vqueue_.front(); //出队的顶点
            vqueue_.pop();
            for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) { //遍历邻接点
                adj_v = *it; //邻接点(右边顶点)
                if (!visited_[adj_v]) {
                    visited_[adj_v] = true;
                    if (right_matching_[adj_v] != NO_VERTEX) { //adj_v已经匹配
                        vqueue_.push(right_matching_[adj_v]); //adj_v匹配到的顶点(左边)入队
                        pre_[right_matching_[adj_v]] = front_v; //记录前驱，即若right_matching_[adj_v]可以找到其他匹配，则front_v与adj_v匹配
                    }
                    else { //adj_v没有匹配
                        Vertex from = front_v, to = adj_v, tmp;
                        while (from != NO_VERTEX) {
                            tmp = left_matching_[from]; //from原来的匹配(右边顶点)
                            left_matching_[from] = to; //from 与 to 匹配
                            right_matching_[to] = from; //from 与 to 匹配
                            //from找到匹配，则pre_[from]与from原来的匹配即tmp匹配，更新from与to进入下次循环,            \
                            若from之前并没有匹配，则pre_[from]为NO_VERTEX，下一次循环就会退出
                            from = pre_[from]; 
                            to = tmp;
                        }
                        while (!vqueue_.empty()) vqueue_.pop(); //清空队列
                        goto NEXT;
                    } //else
                } //if
            } //for
        } //while
    NEXT:
        if (left_matching_[i] != NO_VERTEX) result++;
    } //for
    free(left_matching_);
    free(right_matching_);
    free(pre_);
    free(visited_);
    return result;
}

int main() {
    unsigned int n, m, e;
    scanf("%u %u %u", &n, &m, &e);
    vector<Vertex> * graph = new vector<Vertex>[n];
    Vertex u, v;
    for (size_t i = 0; i < e; i++) {
        scanf("%u %u", &u, &v);
        if (u > n || v > m) continue;
        u--; v--;
        graph[u].push_back(v);
    }
    Hungarian * hungarian;
    //hungarian = new DFS();
    hungarian = new BFS();
    unsigned int max_matching = hungarian->maxMatching(graph, n, m);
    printf("%u", max_matching);
    for (size_t i = 0; i < n; i++)
        vector<Vertex>().swap(graph[i]);
    delete hungarian;
    return 0;
}
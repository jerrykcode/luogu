# P2756 飞行员配对方案问题

[原题地址](https://www.luogu.org/problemnew/show/P2756)

## 题目背景

第二次世界大战时期..

## 题目描述

英国皇家空军从沦陷国征募了大量外籍飞行员。由皇家空军派出的每一架飞机都需要配备在航行技能和语言上能互相配合的2 名飞行员，其中1 名是英国飞行员，另1名是外籍飞行员。在众多的飞行员中，每一名外籍飞行员都可以与其他若干名英国飞行员很好地配合。如何选择配对飞行的飞行员才能使一次派出最多的飞机。对于给定的外籍飞行员与英国飞行员的配合情况，试设计一个算法找出最佳飞行员配对方案，使皇家空军一次能派出最多的飞机。

对于给定的外籍飞行员与英国飞行员的配合情况，编程找出一个最佳飞行员配对方案，使皇家空军一次能派出最多的飞机。

## 输入输出格式

### 输入格式：

第 1 行有 2 个正整数 m 和 n。n 是皇家空军的飞行员总数(n<100)；m 是外籍飞行员数(m<=n)。外籍飞行员编号为 1~m；英国飞行员编号为 m+1~n。

接下来每行有 2 个正整数 i 和 j，表示外籍飞行员 i 可以和英国飞行员 j 配合。最后以 2个-1 结束。

### 输出格式：

第 1 行是最佳飞行员配对方案一次能派出的最多的飞机数 M。接下来 M 行是最佳飞行员配对方案。每行有 2个正整数 i 和 j，表示在最佳飞行员配对方案中，飞行员 i 和飞行员 j 配对。如果所求的最佳飞行员配对方案不存在，则输出‘No Solution!’。

## 输入输出样例

### 输入样例#1： 

> 5 10  
> 1 7  
> 1 8  
> 2 6  
> 2 9  
> 2 10  
> 3 7  
> 3 8  
> 4 7  
> 4 8  
> 5 10  
> -1 -1  

### 输出样例#1： 
> 4  
> 1 7  
> 2 9  
> 3 8  
> 5 10  

## 思路

* 最大匹配，匈牙利算法

二分图分为左右两部分，外籍飞行员作为左边，英国飞行员作为右边，所有边从左边指向右边。输入中左边顶点编号为 1 ~ m，将其改为0 ~ m - 1，右边顶点编号 m + 1 ~ n，将其改为 0 ~ n - m - 1。使用匈牙利算法计算最大匹配，分别用DFS和BFS实现。


## 代码

```cpp
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

typedef unsigned char byte; //1字节类型
typedef byte Vertex; //顶点少于100个，使用byte编号即可

#define BYTE_MAX 0xff
#define NO_VERTEX BYTE_MAX

typedef unsigned int uint;


//匈牙利算法
class Hungarian {
public:
    /* 计算最大匹配 
    @param graph (二分)图，分为左右两部分，所有边从左边顶点指向右边顶点
    @param left_num  左边顶点数量，左边顶点编号 0 ~ left_num - 1
    @param right_num 右边顶点数量，右边顶点编号 0 ~ right_num - 1
    @return void 无返回
    */
    virtual void maxMatching(vector<Vertex> * graph, size_t left_num, size_t right_num) = 0;

    //虚析构函数
    virtual ~Hungarian() {

    }
};


//DFS实现
class DFS : public Hungarian {
public:
    virtual void maxMatching(vector<Vertex> * graph, size_t left_num, size_t right_num);
private:

    /* 对左边的顶点 v 寻找右边的顶点匹配，若找到返回true。*/
    bool dfs(Vertex v);

    vector<Vertex> * graph_;
    bool * visited_; //右边的顶点是否访问过
    Vertex * matching_; //右边顶点匹配到的顶点
};

void DFS::maxMatching(vector<Vertex>* graph, size_t left_num, size_t right_num) {
    graph_ = graph;
    visited_ = new bool[right_num]; //注意大小为right_num
    matching_ = new Vertex[right_num]; //注意大小为right_num
    fill(matching_, matching_ + right_num, NO_VERTEX); //初始化，右边顶点都没有匹配到顶点
    uint num = 0; //匹配数量
    for (size_t i = 0; i < left_num; i++) { //遍历左边顶点
        fill(visited_, visited_ + right_num, false);
        if (dfs(i)) {
            //i找到匹配
            num++;
        }
    }
    printf("%u\n", num);
    if (num > 0) {
        for (size_t i = 0; i < right_num; i++) //遍历右边顶点
            if (matching_[i] != NO_VERTEX) {
                //输出匹配，matching_[i]为左边顶点，i为右边顶点，顶点编号还原，左 + 1，右 + left_num + 1
                printf("%d %d\n", (int)matching_[i] + 1, (int)i + left_num + 1);
            }		
    }
    else {
        printf("No Solution!\n");
    }
    //释放内存
    free(visited_);
    free(matching_);	
}

bool DFS::dfs(Vertex v) {
    Vertex adj_v;
    for (auto it = graph_[v].begin(); it != graph_[v].end(); it++) { //遍历邻接点
        adj_v = *it; //邻接点(右边顶点)
        if (!visited_[adj_v]) { //若没有访问过
            visited_[adj_v] = true;
            if (matching_[adj_v] == NO_VERTEX || dfs(matching_[adj_v])) { //adj_v没有匹配，或者adj_v匹配到的顶点可以找到其他匹配
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
    virtual void maxMatching(vector<Vertex> * graph, size_t left_num, size_t right_num);
private:
    Vertex * left_matching_; //左边顶点匹配到的顶点
    Vertex * right_matching_; //右边顶点匹配到的顶点
    Vertex * pre_; //BFS时左边顶点的前驱
    bool * visited_; //右边顶点是否访问过
    queue<Vertex> vqueue_; //队列
};

void BFS::maxMatching(vector<Vertex>* graph, size_t left_num, size_t right_num) {
    left_matching_ = new Vertex[left_num]; //大小为left_num
    fill(left_matching_, left_matching_ + left_num, NO_VERTEX);
    right_matching_ = new Vertex[right_num]; //大小为right_num
    fill(right_matching_, right_matching_ + right_num, NO_VERTEX);
    pre_ = new Vertex[left_num]; //大小为left_num
    visited_ = new bool[right_num]; //大小为right_num
    uint num = 0; //匹配数量
    for (size_t i = 0; i < left_num; i++) { //遍历左边顶点
        fill(visited_, visited_ + right_num, false);
        pre_[i] = NO_VERTEX; //i没有前驱
        //BFS中进队列的都是左边的顶点，它们的邻接点都是右边顶点
        vqueue_.push((Vertex)i);
        Vertex front_v, adj_v;		
        while (!vqueue_.empty()) {
            front_v = vqueue_.front(); //出队列的顶点
            vqueue_.pop();
            for (auto it = graph[front_v].begin(); it != graph[front_v].end(); it++) { //遍历邻接点
                adj_v = *it; //邻接点(右边顶点)
                if (!visited_[adj_v]) {
                    visited_[adj_v] = true;
                    if (right_matching_[adj_v] != NO_VERTEX) { //adj_v有匹配
                        vqueue_.push(right_matching_[adj_v]); //adj_v匹配到的顶点(左边顶点)入队
                        /*pre_数组: pre_[v]表示v的前驱，即若v找到新的匹配，则pre_[v]可以与v原来的匹配顶点匹配。*/
                        pre_[right_matching_[adj_v]] = front_v; //记录前驱，若right_matching_[adj_v]找到其他匹配，则front_v与adj_v匹配
                    }
                    else { //adj_v没有匹配
                        Vertex from = front_v, to = adj_v;
                        Vertex tmp;
                        while (from != NO_VERTEX) {							
                            tmp = left_matching_[from]; //from原来的匹配
                            left_matching_[from] = to; //from 与 to 匹配
                            right_matching_[to] = from; //from 与 to 匹配
                            //更新，下次循环中，from的前驱与from原来的匹配顶点匹配
                            to = tmp; //to更新为from原来的匹配
                            from = pre_[from]; //from更新为from的前驱							
                        }
                        //上面while循环结束的条件为 from == NO_VERTEX ，即最后一次while循环中，from =  i  ( i 在最外层for循环中)，	\
                        from 更新为 pre_[from]即NO_VERTEX，然后结束while循环。此时 i 已经匹配。
                        while (!vqueue_.empty()) vqueue_.pop(); //清空队列
                        goto NEXT;
                    }
                }
            }
        } //bfs while
    NEXT:
        if (left_matching_[i] != NO_VERTEX) num++;
    } //for
    printf("%u\n", num);
    if (num > 0) {
        for (size_t i = 0; i < left_num; i++)
            if (left_matching_[i] != NO_VERTEX) {
                //输出匹配，顶点编号还原
                printf("%d %d\n", (int)i + 1, (int)left_matching_[i] + left_num + 1);
            }
    }
    else {
        printf("No Solution!\n");
    }
    //释放内存
    free(left_matching_);
    free(right_matching_);
    free(pre_);
    free(visited_);
}

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    vector<Vertex> * graph = new vector<Vertex>[m]; //所有边从左边顶点指向右边顶点
    int i, j;
    while (true) {
        scanf("%d %d", &i, &j);
        if (i == -1) break;
        //输入左边编号 1 ~ m，右边编号 m + 1 ~ n
        //改为: 左边编号 0 ~ m - 1, 右边编号 0 ~ n - m - 1
        i--;
        j -= m + 1;
        graph[(Vertex)i].push_back((Vertex)j);
    }
    //通过 Hungarian指针指向不同的子类来切换算法
    //Hungarian * hungarian = new DFS();
    Hungarian * hungarian = new BFS();
    //Hungarian * hungarian = m % 2 ? (Hungarian *)new DFS() : (Hungarian *)new BFS();
    hungarian->maxMatching(graph, m, n - m);	
    for (size_t i = 0; i < m; i++)
        vector<Vertex>().swap(graph[i]);
    delete hungarian;
    return 0;
}
```
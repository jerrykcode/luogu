#include <cstdio>
#include <cstdlib>
#include <vector>
using std::vector;

#define maxn 100
#define maxq maxn

typedef unsigned char byte;
#define BYTE_UNDEFINED 0xff

//虽然这里是树，但由于输入格式，我们用图的方式表示
typedef struct AdjNode {
    byte adj_v; //邻接点
    unsigned short adj_weight; //与邻接边的权重
    AdjNode(byte adj_v, unsigned short adj_weight) : adj_v(adj_v), adj_weight(adj_weight) {}
} AdjNode;

vector<AdjNode> tree[maxn];
int dp[maxn][maxq]; //dp[v][i]表示以v为根的树，保留i条边可以得到的最大边权和，最终答案即dp[0][q]
int n, q;

int max(int a, int b) {
    return a > b ? a : b;
}

//树形DP
void DP(byte v, byte parent) {
    byte left = BYTE_UNDEFINED, right = BYTE_UNDEFINED; //左右子树
    unsigned short left_weight, right_weight;
    for (auto it = tree[v].begin(); it != tree[v].end(); it++)
        if (it->adj_v != parent) { //不能往回递归
            DP(it->adj_v, v); //递归
            //由于是二叉树，那么任意节点的邻接点中，除了往回的parent，剩下的只能是左子树或者右子树
            //这里给左右子树赋值，谁是左谁是右其实无关紧要
            if (left == BYTE_UNDEFINED) {
                left = it->adj_v;
                left_weight = it->adj_weight;
            }
            else {
                right = it->adj_v;
                right_weight = it->adj_weight;
            }
        }
    //dp[v][0] = 0;
    if (left != BYTE_UNDEFINED /*&& right != BYTE_UNDEFINED*/) { //本题中每个节点要么是叶子，要么有两个子节点
        for (int i = 1; i <= q; i++) { //枚举保留的边数
            dp[v][i] = max(dp[left][i - 1] + left_weight, dp[right][i - 1] + right_weight); //只保留左边，或者只保留右边
            for (int j = 1; j < i; j++) //左右子树都要，左边分配j条边，右边分配i - j条边，枚举j，找到最优值
                dp[v][i] = max(dp[v][i], dp[left][j - 1] + dp[right][i - j - 1] + (int)left_weight + (int)right_weight); //注意通往子节点已经耗费一条边了
        }
    }
}

int main() {
    scanf("%d %d", &n, &q);
    int v, w, e;
    for (int i = 0; i < n - 1; i++) {
        scanf("%d %d %d", &v, &w, &e);
        v--; w--; //改为从0开始编号
        //插入双向边
        tree[v].push_back(AdjNode(w, e));
        tree[w].push_back(AdjNode(v, e));
    }
    DP(0, BYTE_UNDEFINED);
    printf("%d\n", dp[0][q]);
    exit(0);
}

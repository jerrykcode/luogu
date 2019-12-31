#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

template<typename T>
class SBT {
public:
    void insert(T key);
    void remove(T key);
    T rank(int k);

    SBT() : tree_(NULL) {}
    ~SBT() {
        deleteTree(tree_);
    }

private:
    typedef struct TNode {
        T key;
        int size;
        struct TNode *left, *right;
        TNode(T key) : key(key), size(1), left(NULL), right(NULL) {}
    } *Tree;

    int getSize(Tree t) { return t ? t->size : 0; }

    Tree leftRotate(Tree t);
    Tree rightRotate(Tree t);
    Tree maintain(Tree t);
    Tree maintainLeft(Tree t);
    Tree maintainRight(Tree t);
    Tree insert(Tree t, T key);
    Tree remove(Tree t, T key);
    T rank(Tree t, int k);
    void deleteTree(Tree t);

private:
    Tree tree_;
};

struct Query {
    int start, end;
    int k;
    int index;
    static bool compare(Query a, Query b) {
        return a.start < b.start;
    }
};

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    int * arr = new int[n];
    Query * queries = new Query[m];
    for (int i = 0; i < n; i++)
        scanf("%d", arr + i);
    int l, r, k;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &l, &r, &k);
        l--; r--; k--; //l和r减一是为了让下标从0开始，k减一是因为SBT中查询排名第k的元素，也是从0开始排名的。从1开始当然也可以，但我的恶习是从0开始qwq
        queries[i].start = l;
        queries[i].end = r;
        queries[i].k = k;
        queries[i].index = i;
    }
    sort(queries, queries + m, Query::compare);
    int * ans = new int[m];
    SBT<int> sbt;
    int start = 0, end = 0;
    sbt.insert(arr[0]); //初始化
    for (int i = 0; i < m; i++) {
        //start和end表示上一个区间的起止位置
        //queries[i].start和queries[i].end是现在要的区间起止位置
        //for循环外的初始化相当于在最开始增加了[0, 0]区间，目的是为了让代码容易写一点(好像也不一定欸)
        for (int j = start; j <= min(end, queries[i].start - 1); j++) //删除掉上一个区间有而本区间没有的元素，注意这个min可以使得代码在两个区间有无相交的情况下都正常运行
            sbt.remove(arr[j]);
        for (int j = max(end + 1, queries[i].start); j <= queries[i].end; j++) //插入本区有而上一个区间没有的元素，同样注意max
            sbt.insert(arr[j]);
        //至此，SBT中存储的已经是本区间元素了
        start = queries[i].start;
        end = queries[i].end;
        ans[queries[i].index] = sbt.rank(queries[i].k); //查询
    }
    free(arr);
    free(queries);  
    for (int i = 0; i < m; i++)
        printf("%d\n", ans[i]);
    free(ans);
    return 0;
}

//下面都是实现SBT的冗长代码
template<typename T>
void SBT<T>::insert(T key) {
    tree_ = insert(tree_, key);
}

template<typename T>
void SBT<T>::remove(T key) {
    tree_ = remove(tree_, key);
}

template<typename T>
T SBT<T>::rank(int k) {
    return rank(tree_, k);
}

template<typename T>
typename SBT<T>::Tree SBT<T>::leftRotate(SBT<T>::Tree t) {
    Tree k = t->right;
    t->right = k->left;
    k->left = t;
    k->size = t->size;
    t->size = getSize(t->left) + getSize(t->right) + 1;
    return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::rightRotate(SBT<T>::Tree t) {
    Tree k = t->left;
    t->left = k->right;
    k->right = t;
    k->size = t->size;
    t->size = getSize(t->left) + getSize(t->right) + 1;
    return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::maintain(SBT<T>::Tree t) {
    t = maintainLeft(t);
    return maintainRight(t);    
}

template<typename T>
typename SBT<T>::Tree SBT<T>::maintainLeft(SBT<T>::Tree t) {
    if (t == NULL || t->left == NULL) return t;
    if (t->left->left && t->left->left->size > getSize(t->right)) {
        t = rightRotate(t);
        t->right = maintain(t->right);
        t = maintain(t);
    }
    else if (t->left->right && t->left->right->size > getSize(t->right)) {
        t->left = leftRotate(t->left);
        t = rightRotate(t);
        t->left = maintain(t->left);
        t->right = maintain(t->right);
        t = maintain(t);
    }
    return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::maintainRight(SBT<T>::Tree t) {
    if (t == NULL || t->right == NULL) return t;
    if (t->right->right && t->right->right->size > getSize(t->left)) {
        t = leftRotate(t);
        t->left = maintain(t->left);
        t = maintain(t);
    }
    else if (t->right->left && t->right->left->size > getSize(t->left)) {
        t->right = rightRotate(t->right);
        t = leftRotate(t);
        t->left = maintain(t->left);
        t->right = maintain(t->right);
        t = maintain(t);
    }
    return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::insert(SBT<T>::Tree t, T key) {
    if (t == NULL) {
        return new TNode(key);
    }
    t->size++;
    if (key < t->key) {
        t->left = insert(t->left, key);
        t = maintainLeft(t);
    }
    else {
        t->right = insert(t->right, key);
        t = maintainRight(t);
    }
    return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::remove(SBT<T>::Tree t, T key) {
    // (t == NULL) return NULL;
    t->size--;
    if (key == t->key){
        if (t->left && t->right) {
            if (t->left->size > t->right->size) {
                Tree left_max = t->left;
                while (left_max->right) {
                    left_max = left_max->right;
                }
                t->key = left_max->key;
                t->left = remove(t->left, left_max->key);
                t = maintainRight(t);
            }
            else {
                Tree right_min = t->right;
                while (right_min->left) {
                    right_min = right_min->left;
                }
                t->key = right_min->key;
                t->right = remove(t->right, right_min->key);
                t = maintainLeft(t);
            }
        }
        else {
            Tree tmp = t;
            t = t->left ? t->left : t->right;
            delete tmp;
        }
    }
    else if (key < t->key) {
        t->left = remove(t->left, key);
        t = maintainRight(t);
    }
    else {
        t->right = remove(t->right, key);
        t = maintainLeft(t);
    }
    return t;
}

//从0开始排名，好处是排名k的意思就是有k个元素小于它，从1开始排名那就是k-1了。。
template<typename T>
T SBT<T>::rank(SBT<T>::Tree t, int k) {
    //if (t == NULL) return 0xffffffff;
    int left_size = getSize(t->left);
    if (k == left_size) {
        return t->key;
    }
    else if (k < left_size) {
        return rank(t->left, k);
    }
    else {
        return rank(t->right, k - left_size - 1);
    }
}

template<typename T>
void SBT<T>::deleteTree(SBT<T>::Tree t) {
    if (t == NULL) {
        return;
    }
    deleteTree(t->left);
    deleteTree(t->right);
    delete t;
}

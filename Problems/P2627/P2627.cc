#include <cstdio>
#include <cstdlib>

template<typename T>
class Queue {
public:
    Queue(size_t capacity) : capacity_(capacity), size_(0), head_(NULL), tail_(NULL) {}
    ~Queue();
    bool Push(T element, T* p_out_element);
private:
    typedef struct QNode {
        T element;
        struct QNode *next, *pre;
        QNode(T element) : element(element), next(NULL), pre(NULL) {}
    } *QList;
private:
    size_t capacity_;
    size_t size_;
    QList head_, tail_;
};

template<typename T>
class SBT {
public:
    SBT() : t_(NULL) {}
    ~SBT() {
        DeleteTree(t_);
    }
    void Insert(T key);
    void Remove(T key);
    T MaxKey();
private:
    typedef struct TNode {
        T key;
        size_t size;
        struct TNode *left, *right;
        TNode(T key) : key(key), size(1), left(NULL), right(NULL) {}
    } *Tree;

    size_t GetSize(Tree t) { return t ? t->size : 0; }

    Tree LeftRotate(Tree t);
    Tree RightRotate(Tree t);
    
    Tree Maintain(Tree t);
    Tree MaintainLeft(Tree t);
    Tree MaintainRight(Tree t);

    Tree Insert(Tree t, T key);
    Tree Remove(Tree t, T key);
    T MaxKey(Tree t);

    void DeleteTree(Tree t);
private:
    Tree t_;
};

typedef long long ll;

int main() {
    int n, k, e;
    scanf("%d %d", &n, &k);
    SBT<ll> sbt;
    Queue<ll> queue(k + 1);
    ll total = 0;
    ll maxKey, rmKey;
    sbt.Insert(0);
    queue.Push(0, &rmKey);
    for (int i = 0; i < n; i++) {
        scanf("%d", &e);
        total += e;
        maxKey = sbt.MaxKey();
        sbt.Insert(maxKey - e);
        if (queue.Push(maxKey - e, &rmKey))
            sbt.Remove(rmKey);
    }
    printf("%lld", sbt.MaxKey() + total);
    return 0;
}

template<typename T>
Queue<T>::~Queue() {
    QList tmp;
    while (head_) {
        tmp = head_;
        head_ = head_->next;
        delete tmp;
    }
}

template<typename T>
bool Queue<T>::Push(T element, T * p_out_element) {
    QList q = new QNode(element);
    if (!head_) {
        head_ = tail_ = q;
        size_ = 1;
        return false;
    }
    q->next = head_;
    head_->pre = q;
    head_ = q;
    if (size_ < capacity_) {
        size_++;
        return false;
    }
    else {
        *p_out_element = tail_->element;
        tail_ = tail_->pre;
        delete tail_->next;
        tail_->next = NULL;
        return true;
    }
}

template<typename T>
void SBT<T>::Insert(T key) {
    t_ = Insert(t_, key);
}

template<typename T>
void SBT<T>::Remove(T key) {
    t_ = Remove(t_, key);
}

template<typename T>
T SBT<T>::MaxKey() {
    return MaxKey(t_);
}

template<typename T>
typename SBT<T>::Tree SBT<T>::LeftRotate(Tree t) {
    Tree k = t->right;
    t->right = k->left;
    k->left = t;
    k->size = t->size;
    t->size = GetSize(t->left) + GetSize(t->right) + 1;
    return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::RightRotate(Tree t) {
    Tree k = t->left;
    t->left = k->right;
    k->right = t;
    k->size = t->size;
    t->size = GetSize(t->left) + GetSize(t->right) + 1;
    return k;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::Maintain(Tree t) {
    t = MaintainLeft(t);
    return MaintainRight(t);
}

template<typename T>
typename SBT<T>::Tree SBT<T>::MaintainLeft(Tree t) {
    if (!t || !t->left) return t;
    if (t->left->left && t->left->left->size > GetSize(t->right)) {
        t = RightRotate(t);
        t->right = Maintain(t->right);
        t = Maintain(t);
    }
    if (t->left->right && t->left->right->size > GetSize(t->right)) {
        t->left = LeftRotate(t->left);
        t = RightRotate(t);
        t->left = Maintain(t->left);
        t->right = Maintain(t->right);
        t = Maintain(t);
    }
    return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::MaintainRight(Tree t) {
    if (!t || !t->right) return t;
    if (t->right->right && t->right->right->size > GetSize(t->left)) {
        t = LeftRotate(t);
        t->left = Maintain(t->left);
        t = Maintain(t);
    }
    if (t->right->left && t->right->left->size > GetSize(t->left)) {
        t->right = RightRotate(t->right);
        t = LeftRotate(t);
        t->left = Maintain(t->left);
        t->right = Maintain(t->right);
        t = Maintain(t);
    }
    return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::Insert(Tree t, T key) {
    if (!t) {
        return new TNode(key);
    }
    t->size++;
    if (key < t->key) {
        t->left = Insert(t->left, key);
        t = MaintainLeft(t);
    }
    else{
        t->right = Insert(t->right, key);
        t = MaintainRight(t);
    }
    return t;
}

template<typename T>
typename SBT<T>::Tree SBT<T>::Remove(Tree t, T key) {
    if (!t) return t;    
    if (key == t->key) {
        if (!t->left && !t->right) return NULL;
        if (t->left && !t->right) return t->left;
        if (!t->left && t->right) return t->right;
        t->size--;
        Tree leftMax = t->left;
        while (leftMax->right) {
            leftMax = leftMax->right;
        }
        t->key = leftMax->key;
        t->left = Remove(t->left, leftMax->key);
        t = MaintainRight(t);
    }
    else if (key < t->key) {
        t->size--;
        t->left = Remove(t->left, key);
        t = MaintainRight(t);
    }
    else {
        t->size--;
        t->right = Remove(t->right, key);
        t = MaintainLeft(t);
    }
    return t;
}

template<typename T>
T SBT<T>::MaxKey(Tree t) {
    //t不会是NULL
    while (t->right) {
        t = t->right;
    }
    return t->key;
}

template<typename T>
void SBT<T>::DeleteTree(Tree t) {
    if (!t) return;
    DeleteTree(t->left);
    DeleteTree(t->right);
    delete t;
}

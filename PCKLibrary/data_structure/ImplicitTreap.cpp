//暗黙 Treap。挿入・削除・一点更新・反転・区間積は期待 O(log N)
//0-indexed、半開区間 [l, r)。prod は非可換演算に対応
template<class S, S(*op)(S, S), S(*e)()>
struct ImplicitTreap {
    struct Node {
        S value, prod, revProd;
        unsigned priority;
        int siz;
        bool rev;
        Node *left, *right;

        Node(S x, unsigned p) : value(x), prod(x), revProd(x), priority(p),
                               rev(false), left(nullptr), right(nullptr) {
            siz = 1;
        }
    };
    Node *root = nullptr;
    unsigned seed = 2463534242U;

    int size(Node *t) const { return t == nullptr ? 0 : t->siz; }
    S prod(Node *t) const { return t == nullptr ? e() : t->prod; }
    S revProd(Node *t) const { return t == nullptr ? e() : t->revProd; }
    unsigned random() {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return seed;
    }
    void applyReverse(Node *t) {
        if (t == nullptr) return;
        swap(t->left, t->right);
        swap(t->prod, t->revProd);
        t->rev ^= true;
    }
    void push(Node *t) {
        if (t == nullptr or !t->rev) return;
        applyReverse(t->left);
        applyReverse(t->right);
        t->rev = false;
    }
    void update(Node *t) {
        if (t == nullptr) return;
        t->siz = 1 + size(t->left) + size(t->right);
        t->prod = op(op(prod(t->left), t->value), prod(t->right));
        t->revProd = op(op(revProd(t->right), t->value), revProd(t->left));
    }
    pair<Node*, Node*> split(Node *t, int k) {
        if (t == nullptr) return {nullptr, nullptr};
        push(t);
        if (k <= size(t->left)) {
            auto [a, b] = split(t->left, k);
            t->left = b;
            update(t);
            return {a, t};
        }
        auto [a, b] = split(t->right, k - size(t->left) - 1);
        t->right = a;
        update(t);
        return {t, b};
    }
    Node *merge(Node *a, Node *b) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;
        if (a->priority > b->priority) {
            push(a);
            a->right = merge(a->right, b);
            update(a);
            return a;
        }
        push(b);
        b->left = merge(a, b->left);
        update(b);
        return b;
    }
    int size() const { return size(root); }
    void insert(int k, S x) {
        assert(0 <= k and k <= size());
        auto [a, b] = split(root, k);
        root = merge(merge(a, new Node(x, random())), b);
    }
    S erase(int k) {
        assert(0 <= k and k < size());
        auto [a, c] = split(root, k);
        auto [b, d] = split(c, 1);
        S ret = b->value;
        root = merge(a, d);
        return ret;
    }
    void set(int k, S x) {
        assert(0 <= k and k < size());
        auto [a, c] = split(root, k);
        auto [b, d] = split(c, 1);
        b->value = x;
        update(b);
        root = merge(a, merge(b, d));
    }
    S get(int k) { return prod(k, k + 1); }
    void reverse(int l, int r) {
        assert(0 <= l and l <= r and r <= size());
        auto [a, c] = split(root, l);
        auto [b, d] = split(c, r - l);
        applyReverse(b);
        root = merge(a, merge(b, d));
    }
    S prod(int l, int r) {
        assert(0 <= l and l <= r and r <= size());
        auto [a, c] = split(root, l);
        auto [b, d] = split(c, r - l);
        S ret = prod(b);
        root = merge(a, merge(b, d));
        return ret;
    }
};

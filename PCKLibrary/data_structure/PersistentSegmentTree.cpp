//永続 Segment Tree。一点代入・区間積・二版間の k 番目を O(log N)
//0-indexed、半開区間。kth は S が個数で、rightRoot - leftRoot の k 番目
template<class S, S(*op)(S, S), S(*e)()>
struct PersistentSegmentTree {
    struct Node {
        S value;
        int left = -1, right = -1;
    };
    int N, siz;
    vector<Node> node;

    PersistentSegmentTree(int n) : N(n) {
        siz = 1;
        while (siz < N) siz <<= 1;
    }
    int makeNode(S x = e(), int l = -1, int r = -1) {
        node.push_back({x, l, r});
        return (int)node.size() - 1;
    }
    int build(const vector<S>& A, int l, int r) {
        if (l + 1 == r) return makeNode(l < N ? A[l] : e());
        int m = (l + r) / 2;
        int a = build(A, l, m), b = build(A, m, r);
        return makeNode(op(node[a].value, node[b].value), a, b);
    }
    int build(const vector<S>& A) {
        assert((int)A.size() == N);
        return build(A, 0, siz);
    }
    int set(int root, int p, S x, int l, int r) {
        if (l + 1 == r) return makeNode(x);
        int m = (l + r) / 2;
        int a = node[root].left, b = node[root].right;
        if (p < m) a = set(a, p, x, l, m);
        else b = set(b, p, x, m, r);
        return makeNode(op(node[a].value, node[b].value), a, b);
    }
    int set(int root, int p, S x) {
        assert(0 <= p and p < N);
        return set(root, p, x, 0, siz);
    }
    S prod(int root, int ql, int qr, int l, int r) const {
        if (r <= ql or qr <= l) return e();
        if (ql <= l and r <= qr) return node[root].value;
        int m = (l + r) / 2;
        return op(prod(node[root].left, ql, qr, l, m),
                  prod(node[root].right, ql, qr, m, r));
    }
    S prod(int root, int l, int r) const {
        assert(0 <= l and l <= r and r <= N);
        return prod(root, l, r, 0, siz);
    }
    int kth(int leftRoot, int rightRoot, S k) const {
        assert(S(0) <= k and k < node[rightRoot].value - node[leftRoot].value);
        int a = leftRoot, b = rightRoot, l = 0, r = siz;
        while (l + 1 < r) {
            S cnt = node[node[b].left].value - node[node[a].left].value;
            int m = (l + r) / 2;
            if (k < cnt) {
                a = node[a].left; b = node[b].left; r = m;
            } else {
                k -= cnt;
                a = node[a].right; b = node[b].right; l = m;
            }
        }
        return l;
    }
};

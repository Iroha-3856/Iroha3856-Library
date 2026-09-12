// 永続 Segment Tree。一点代入・区間積・二版間の k 番目を O(log N)
// 0-indexed、半開区間。kth は S が個数で、rightRoot - leftRoot の k 番目
// 使い方:
// PersistentSegmentTree<S, op, e> seg(N); int root0 = seg.build(A); で初版を作る。
// int root1 = seg.set(root0, p, x); のように返り値を各版の root として保存する。
// seg.prod(root, l, r) は指定版の区間積。kth(leftRoot, rightRoot, k) の k は 0-indexed。
// kth は各葉が非負個数、op が加算である頻度 Segment Tree にだけ使用する。
// 使いどころ: 更新前の配列も参照する、または prefix ごとの頻度木から区間 k 番目を求める場合。
// 具体例: root1=set(root0, 2, 5) としても root0 の位置2は変わらず、root1 だけが 5 を持つ。
// root はポインタの代わりとなる node 配列の添字であり、版ごとに呼出側が保存する。
template<class S, S(*op)(S, S), S(*e)()>
struct PersistentSegmentTree {
    struct Node {
        S value;
        int left = -1, right = -1;
    };
    int N, siz;
    vector<Node> node;

    // 長さ n の完全二分木サイズを決める。版の根は build で作る。
    PersistentSegmentTree(int n) : N(n) {
        siz = 1;
        while (siz < N) siz <<= 1;
    }
    // 値 x と子 l, r を持つ節点を追加し、その node 添字を返す。
    int makeNode(S x = e(), int l = -1, int r = -1) {
        node.push_back({x, l, r});
        return (int)node.size() - 1;
    }
    // 担当区間 [l, r) の初期木を再帰構築し、その根を返す内部関数。
    int build(const vector<S>& A, int l, int r) {
        if (l + 1 == r) return makeNode(l < N ? A[l] : e());
        int m = (l + r) / 2;
        int a = build(A, l, m), b = build(A, m, r);
        return makeNode(op(node[a].value, node[b].value), a, b);
    }
    // 長さ N の A から初版を構築し、版を表す根の添字を返す。
    int build(const vector<S>& A) {
        assert((int)A.size() == N);
        return build(A, 0, siz);
    }
    // root 版の位置 p だけを x に変え、区間 [l, r) の新しい根を返す内部関数。
    int set(int root, int p, S x, int l, int r) {
        if (l + 1 == r) return makeNode(x);
        int m = (l + r) / 2;
        int a = node[root].left, b = node[root].right;
        if (p < m) a = set(a, p, x, l, m);
        else b = set(b, p, x, m, r);
        return makeNode(op(node[a].value, node[b].value), a, b);
    }
    // root 版の位置 p を x に置き換えた新しい版の根を返す。元版は変更しない。
    int set(int root, int p, S x) {
        assert(0 <= p and p < N);
        return set(root, p, x, 0, siz);
    }
    // root 版の問合せ [ql, qr) を担当区間 [l, r) から集計する内部関数。
    S prod(int root, int ql, int qr, int l, int r) const {
        if (r <= ql or qr <= l) return e();
        if (ql <= l and r <= qr) return node[root].value;
        int m = (l + r) / 2;
        return op(prod(node[root].left, ql, qr, l, m),
                  prod(node[root].right, ql, qr, m, r));
    }
    // root 版の半開区間 [l, r) の積を返す。
    S prod(int root, int l, int r) const {
        assert(0 <= l and l <= r and r <= N);
        return prod(root, l, r, 0, siz);
    }
    // rightRoot-leftRoot の頻度差における 0-indexed の k 番目の葉添字を返す。
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

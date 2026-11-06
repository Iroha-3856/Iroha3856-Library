// 動的 Segment Tree。巨大な整数添字範囲の一点代入・一点作用・区間積を O(log(R-L))
// 0-indexed の配列に限らず、任意の long long 半開区間 [L, R) を扱う。
// 使いどころ: 座標範囲が巨大で、更新される位置だけに節点を確保したい場合。
// 使い方:
// ll op(ll a, ll b) { return a + b; } ll e() { return 0; }
// DynamicSegmentTree<ll, op, e> seg(-1000000000LL, 1000000000LL);
// seg.set(3, 5); seg.apply(3, 2); // prod(0, 4) == 7、get(3) == 7
// 未更新位置の値は e()。非可換 op でも添字の昇順に結合する。
// R-L は __int128_t で計算するため、long long の全域に対応する。
// 節点数は O(異なる更新点数 × log(R-L))。既存節点は削除しない。
template<class S, S(*op)(S, S), S(*e)()>
struct DynamicSegmentTree {
    struct Node {
        S value = e();
        int left = -1, right = -1;
    };
    ll L, R;
    int root = -1;
    vector<Node> node;

    // 添字範囲 [l, r) の空の木を作る。l < r が必要。
    DynamicSegmentTree(ll l, ll r) : L(l), R(r) {
        assert(L < R);
    }
    // オーバーフローせず区間 [l, r) を二分する。
    static ll midpoint(ll l, ll r) {
        return (ll)((__int128_t)l + ((__int128_t)r - l) / 2);
    }
    // 担当区間 [l, r) の位置 p を更新し、節点の添字を返す。
    // add=true のときは現在値の右に x を結合する。
    int update(int k, ll l, ll r, ll p, S x, bool add) {
        if (k == -1) {
            k = (int)node.size();
            node.push_back(Node());
        }
        if (l + 1 == r) {
            node[k].value = add ? op(node[k].value, x) : x;
            return k;
        }
        ll m = midpoint(l, r);
        if (p < m) {
            int child = update(node[k].left, l, m, p, x, add);
            node[k].left = child;
        } else {
            int child = update(node[k].right, m, r, p, x, add);
            node[k].right = child;
        }
        S a = node[k].left == -1 ? e() : node[node[k].left].value;
        S b = node[k].right == -1 ? e() : node[node[k].right].value;
        node[k].value = op(a, b);
        return k;
    }
    // 位置 p の値を x に置き換える。
    void set(ll p, S x) {
        assert(L <= p and p < R);
        root = update(root, L, R, p, x, false);
    }
    // 位置 p の現在値を op(現在値, x) に置き換える。
    void apply(ll p, S x) {
        assert(L <= p and p < R);
        root = update(root, L, R, p, x, true);
    }
    // 節点 k から区間 [ql, qr) の積を添字の昇順に求める。
    S prod(int k, ll l, ll r, ll ql, ll qr) const {
        if (k == -1 or r <= ql or qr <= l) return e();
        if (ql <= l and r <= qr) return node[k].value;
        ll m = midpoint(l, r);
        return op(prod(node[k].left, l, m, ql, qr),
                  prod(node[k].right, m, r, ql, qr));
    }
    // 半開区間 [l, r) の積を返す。空区間なら e()。
    S prod(ll l, ll r) const {
        assert(L <= l and l <= r and r <= R);
        return prod(root, L, R, l, r);
    }
    // 位置 p の現在値を返す。未更新なら e()。
    S get(ll p) const {
        assert(L <= p and p < R);
        return prod(p, p + 1);
    }
    // 全域の積を返す。
    S allProd() const {
        return root == -1 ? e() : node[root].value;
    }
};

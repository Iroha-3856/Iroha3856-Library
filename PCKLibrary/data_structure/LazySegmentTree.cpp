// 非再帰 Lazy Segment Tree
// composition(f, g) は f(g(x))、各操作 O(log N)
// 使い方:
// 値モノイド S の op/e、作用 F の mapping/composition/id を定義して型を作る。
// apply(l, r, f) で [l, r) へ作用し、prod(l, r) で区間積を得る。
// maxRight(l, g) / minLeft(r, g) は g(e()) == true かつ g が単調なときだけ使う。
// range add / range min の具体的な型と関数はファイル末尾のコメントを参照する。
// 使いどころ: 区間全体への同じ更新と、区間集計をどちらも O(log N) で行う場合。
// 具体例: 末尾の RangeAddRangeMin では apply(1, 4, 3) が A[1..3] に 3 を加え、prod(0, 4) が最小値。
// S は区間情報、F は遅延作用。mapping(f, x) は情報 x へ f を適用した結果を返す。
template<class S, S(*op)(S, S), S(*e)(), class F,
         S(*mapping)(F, S), F(*composition)(F, F), F(*id)()>
struct LazySegmentTree {
    int N, siz, log;
    vector<S> data;
    vector<F> lazy;

    // 初期配列 A から木を構築する。空配列も構築できる。
    LazySegmentTree(const vector<S>& A) {
        N = (int)A.size();
        siz = 1; log = 0;
        while (siz < N) siz <<= 1, log++;
        data.assign(2 * siz, e());
        lazy.assign(siz, id());
        for (int i = 0; i < N; i++) data[siz + i] = A[i];
        for (int i = siz - 1; i; i--) update(i);
    }
    // 二つの子の積から内部節点 k の値を再計算する。
    void update(int k) {
        data[k] = op(data[2 * k], data[2 * k + 1]);
    }
    // 節点 k 全体へ作用 f を適用し、内部節点なら遅延作用も合成する。
    void allApply(int k, F f) {
        data[k] = mapping(f, data[k]);
        if (k < siz) lazy[k] = composition(f, lazy[k]);
    }
    // 節点 k の遅延作用を二つの子へ伝播し、k を id() に戻す。
    void push(int k) {
        allApply(2 * k, lazy[k]);
        allApply(2 * k + 1, lazy[k]);
        lazy[k] = id();
    }
    // 半開区間 [l, r) に作用 f を適用する。空区間では何もしない。
    void apply(int l, int r, F f) {
        if (l == r) return;
        l += siz; r += siz;
        for (int i = log; i; i--) {
            if ((l >> i) << i != l) push(l >> i);
            if ((r >> i) << i != r) push(r >> i);
        }
        int l2 = l, r2 = r;
        while (l < r) {
            if (l & 1) allApply(l++, f);
            if (r & 1) allApply(--r, f);
            l >>= 1; r >>= 1;
        }
        l = l2; r = r2;
        for (int i = 1; i <= log; i++) {
            if ((l >> i) << i != l) update(l >> i);
            if ((r >> i) << i != r) update(r >> i);
        }
    }
    // 半開区間 [l, r) の積を返す。空区間なら e()。
    S prod(int l, int r) {
        if (l == r) return e();
        l += siz; r += siz;
        for (int i = log; i; i--) {
            if ((l >> i) << i != l) push(l >> i);
            if ((r >> i) << i != r) push(r >> i);
        }
        S left = e(), right = e();
        while (l < r) {
            if (l & 1) left = op(left, data[l++]);
            if (r & 1) right = op(data[--r], right);
            l >>= 1; r >>= 1;
        }
        return op(left, right);
    }
    // g(prod(l, r)) が真となる最大の r を返す。g(e())=true と単調性が必要。
    template<class G>
    int maxRight(int l, G g) {
        if (l == N) return N;
        l += siz;
        for (int i = log; i; i--) push(l >> i);
        S value = e();
        do {
            while ((l & 1) == 0) l >>= 1;
            if (!g(op(value, data[l]))) {
                while (l < siz) {
                    push(l);
                    l <<= 1;
                    if (g(op(value, data[l]))) {
                        value = op(value, data[l]);
                        l++;
                    }
                }
                return min(N, l - siz);
            }
            value = op(value, data[l]);
            l++;
        } while ((l & -l) != l);
        return N;
    }
    // g(prod(l, r)) が真となる最小の l を返す。g(e())=true と単調性が必要。
    template<class G>
    int minLeft(int r, G g) {
        if (r == 0) return 0;
        r += siz;
        for (int i = log; i; i--) push((r - 1) >> i);
        S value = e();
        do {
            r--;
            while (r > 1 and (r & 1)) r >>= 1;
            if (!g(op(data[r], value))) {
                while (r < siz) {
                    push(r);
                    r = 2 * r + 1;
                    if (g(op(data[r], value))) {
                        value = op(data[r], value);
                        r--;
                    }
                }
                return r + 1 - siz;
            }
            value = op(data[r], value);
        } while ((r & -r) != r);
        return 0;
    }
};

// 使用例: 区間加算・区間最小値
namespace RangeAddRangeMin {
    using S = long long;
    using F = long long;
    // 区間最小値を併合する。
    S op(S a, S b) { return min(a, b); }
    // 区間最小値モノイドの単位元を返す。
    S e() { return INF; }
    // 区間最小値 x へ区間加算 f を反映する。
    S mapping(F f, S x) { return x == INF ? INF : x + f; }
    // 先に g、その後 f の区間加算を一つへ合成する。
    F composition(F f, F g) { return f + g; }
    // 何も加算しない恒等作用を返す。
    F id() { return 0; }
    using SegTree = LazySegmentTree<S, op, e, F, mapping, composition, id>;
}

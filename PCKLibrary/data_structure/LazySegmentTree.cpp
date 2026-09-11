//非再帰 Lazy Segment Tree
//composition(f, g) は f(g(x))、各操作 O(log N)
template<class S, S(*op)(S, S), S(*e)(), class F,
         S(*mapping)(F, S), F(*composition)(F, F), F(*id)()>
struct LazySegmentTree {
    int N, siz, log;
    vector<S> data;
    vector<F> lazy;

    LazySegmentTree(const vector<S>& A) {
        N = (int)A.size();
        siz = 1; log = 0;
        while (siz < N) siz <<= 1, log++;
        data.assign(2 * siz, e());
        lazy.assign(siz, id());
        for (int i = 0; i < N; i++) data[siz + i] = A[i];
        for (int i = siz - 1; i; i--) update(i);
    }
    void update(int k) {
        data[k] = op(data[2 * k], data[2 * k + 1]);
    }
    void allApply(int k, F f) {
        data[k] = mapping(f, data[k]);
        if (k < siz) lazy[k] = composition(f, lazy[k]);
    }
    void push(int k) {
        allApply(2 * k, lazy[k]);
        allApply(2 * k + 1, lazy[k]);
        lazy[k] = id();
    }
    //区間 [l, r) に f を作用させる
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
    //区間 [l, r) の積
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
    //g(prod(l, r)) が true となる最大の r。g(e()) == true が必要
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
    //g(prod(l, r)) が true となる最小の l。g(e()) == true が必要
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

//使用例: 区間加算・区間最小値
namespace RangeAddRangeMin {
    using S = long long;
    using F = long long;
    S op(S a, S b) { return min(a, b); }
    S e() { return INF; }
    S mapping(F f, S x) { return x == INF ? INF : x + f; }
    F composition(F f, F g) { return f + g; }
    F id() { return 0; }
    using SegTree = LazySegmentTree<S, op, e, F, mapping, composition, id>;
}

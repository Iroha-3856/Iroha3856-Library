// 一点更新・区間積。O(log N)
// 使い方:
// 結合則を満たす T op(T, T) と単位元 T e() を定義し、SegmentTree<T, op, e> seg(A);
// set(p, x), get(p), prod(l, r) を使う。区間はすべて 0-indexed の [l, r)。
// maxRight / minLeft は f(e()) == true の単調述語で、条件を満たす最長境界を探す。
// 使いどころ: 点更新と区間集計を繰り返す標準的な固定長配列。区間更新が必要なら Lazy 版を使う。
// 具体例: A={2, 1, 4}、op=加算、e=0 なら prod(1, 3)=5。set(1, 3) 後は prod(0, 3)=9。
// 非可換 op にも対応し、prod は必ず A[l] から A[r-1] の順に結合する。
template<class T, T(*op)(T, T), T(*e)()>
struct SegmentTree {
    int N, siz;
    vector<T> data;

    // 長さ n、全要素 e() の Segment Tree を構築する。
    SegmentTree(int n) {
        N = n;
        siz = 1;
        while (siz < n) siz <<= 1;
        data.assign(2 * siz, e());
    }
    // 配列 A の各要素を葉として Segment Tree を構築する。
    SegmentTree(const vector<T>& A) : SegmentTree((int)A.size()) {
        for (int i = 0; i < (int)A.size(); i++) data[siz + i] = A[i];
        for (int i = siz - 1; i; i--) data[i] = op(data[2 * i], data[2 * i + 1]);
    }
    // 添字 p の値を x に置き換える。
    void set(int p, T x) {
        p += siz;
        data[p] = x;
        while (p >>= 1) data[p] = op(data[2 * p], data[2 * p + 1]);
    }
    // 添字 p の現在値を返す。
    T get(int p) {
        return data[p + siz];
    }
    // 半開区間 [l, r) を左から順に畳み込む。空区間は e()。
    T prod(int l, int r) {
        T left = e(), right = e();
        l += siz; r += siz;
        while (l < r) {
            if (l & 1) left = op(left, data[l++]);
            if (r & 1) right = op(data[--r], right);
            l >>= 1; r >>= 1;
        }
        return op(left, right);
    }
    // maxRight 公開版から使う内部 DFS。value に確定済みの左側積を保持する。
    template<class F>
    int maxRight(int a, F f, int k, int l, int r, T& value) {
        if (a <= l) {
            T next = op(value, data[k]);
            if (f(next)) {
                value = next;
                return r;
            }
            if (r - l == 1) return l;
        }
        int m = (l + r) / 2;
        if (a < m) {
            int ret = maxRight(a, f, 2 * k, l, m, value);
            if (ret < m) return ret;
        }
        return maxRight(a, f, 2 * k + 1, m, r, value);
    }
    // f(prod(l, r)) が true となる最大の r を返す。f(e()) == true と単調性が必要。
    template<class F>
    int maxRight(int l, F f) {
        if (l == N) return N;
        T value = e();
        return min(N, maxRight(l, f, 1, 0, siz, value));
    }
    // minLeft 公開版から使う内部 DFS。value に確定済みの右側積を保持する。
    template<class F>
    int minLeft(int b, F f, int k, int l, int r, T& value) {
        if (r <= b) {
            T next = op(data[k], value);
            if (f(next)) {
                value = next;
                return l;
            }
            if (r - l == 1) return r;
        }
        int m = (l + r) / 2;
        if (m < b) {
            int ret = minLeft(b, f, 2 * k + 1, m, r, value);
            if (ret > m) return ret;
        }
        return minLeft(b, f, 2 * k, l, m, value);
    }
    // f(prod(l, r)) が true となる最小の l を返す。f(e()) == true と単調性が必要。
    template<class F>
    int minLeft(int r, F f) {
        if (r == 0) return 0;
        T value = e();
        return minLeft(r, f, 1, 0, siz, value);
    }
};

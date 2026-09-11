//一点更新・区間積。O(log N)
template<class T, T(*op)(T, T), T(*e)()>
struct SegmentTree {
    int N, siz;
    vector<T> data;

    SegmentTree(int n) {
        N = n;
        siz = 1;
        while (siz < n) siz <<= 1;
        data.assign(2 * siz, e());
    }
    SegmentTree(const vector<T>& A) : SegmentTree((int)A.size()) {
        for (int i = 0; i < (int)A.size(); i++) data[siz + i] = A[i];
        for (int i = siz - 1; i; i--) data[i] = op(data[2 * i], data[2 * i + 1]);
    }
    void set(int p, T x) {
        p += siz;
        data[p] = x;
        while (p >>= 1) data[p] = op(data[2 * p], data[2 * p + 1]);
    }
    T get(int p) {
        return data[p + siz];
    }
    //[l, r)
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
    //f(prod(l, r)) が true となる最大の r。f(e()) == true が必要
    template<class F>
    int maxRight(int l, F f) {
        if (l == N) return N;
        T value = e();
        return min(N, maxRight(l, f, 1, 0, siz, value));
    }
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
    //f(prod(l, r)) が true となる最小の l。f(e()) == true が必要
    template<class F>
    int minLeft(int r, F f) {
        if (r == 0) return 0;
        T value = e();
        return minLeft(r, f, 1, 0, siz, value);
    }
};

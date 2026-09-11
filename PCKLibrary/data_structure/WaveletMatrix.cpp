//静的列の Wavelet Matrix。rank / kth / rangeFreq / prev / next は O(bit 数)
//0-indexed、半開区間 [l, r)。符号付き整数も順序を保って扱う
template<class T>
struct WaveletMatrix {
    using U = make_unsigned_t<T>;
    static constexpr int B = numeric_limits<U>::digits;
    int N;
    vector<int> mid;
    vector<vector<int>> bit;

    U encode(T x) const {
        U u = (U)x;
        if constexpr (is_signed_v<T>) u ^= U(1) << (B - 1);
        return u;
    }
    T decode(U x) const {
        if constexpr (is_signed_v<T>) x ^= U(1) << (B - 1);
        return (T)x;
    }
    WaveletMatrix(const vector<T>& A) : N((int)A.size()), mid(B), bit(B) {
        vector<U> cur(N), next(N);
        for (int i = 0; i < N; i++) cur[i] = encode(A[i]);
        for (int h = B - 1; h >= 0; h--) {
            bit[h].assign(N + 1, 0);
            for (int i = 0; i < N; i++) bit[h][i + 1] = bit[h][i] + (cur[i] >> h & 1);
            mid[h] = N - bit[h][N];
            int p0 = 0, p1 = mid[h];
            for (U x : cur) {
                if (x >> h & 1) next[p1++] = x;
                else next[p0++] = x;
            }
            cur.swap(next);
        }
    }
    int rank(T x, int l, int r) const {
        assert(0 <= l and l <= r and r <= N);
        U y = encode(x);
        for (int h = B - 1; h >= 0; h--) {
            int ol = bit[h][l], ore = bit[h][r];
            if (y >> h & 1) l = mid[h] + ol, r = mid[h] + ore;
            else l -= ol, r -= ore;
        }
        return r - l;
    }
    T kth(int l, int r, int k) const {
        assert(0 <= l and l <= r and r <= N and 0 <= k and k < r - l);
        U ret = 0;
        for (int h = B - 1; h >= 0; h--) {
            int ol = bit[h][l], ore = bit[h][r];
            int zero = (r - l) - (ore - ol);
            if (k < zero) l -= ol, r -= ore;
            else {
                k -= zero;
                ret |= U(1) << h;
                l = mid[h] + ol; r = mid[h] + ore;
            }
        }
        return decode(ret);
    }
    int lessThan(int l, int r, T x) const {
        assert(0 <= l and l <= r and r <= N);
        U y = encode(x);
        int ret = 0;
        for (int h = B - 1; h >= 0; h--) {
            int ol = bit[h][l], ore = bit[h][r];
            int zero = (r - l) - (ore - ol);
            if (y >> h & 1) {
                ret += zero;
                l = mid[h] + ol; r = mid[h] + ore;
            } else {
                l -= ol; r -= ore;
            }
        }
        return ret;
    }
    int rangeFreq(int l, int r, T low, T high) const {
        return lessThan(l, r, high) - lessThan(l, r, low);
    }
    optional<T> prevValue(int l, int r, T upper) const {
        int k = lessThan(l, r, upper);
        if (k == 0) return nullopt;
        return kth(l, r, k - 1);
    }
    optional<T> nextValue(int l, int r, T lower) const {
        int k = lessThan(l, r, lower);
        if (k == r - l) return nullopt;
        return kth(l, r, k);
    }
};

// 静的列の Wavelet Matrix。rank / kth / rangeFreq / prev / next は O(bit 数)
// 0-indexed、半開区間 [l, r)。符号付き整数も順序を保って扱う
// 使い方:
// WaveletMatrix<int> wm(A); とし、rank(x, l, r) で x の個数、kth(l, r, k) で k 番目を得る。
// rangeFreq(l, r, low, high) は値域 [low, high) の個数を返す。
// prevValue(..., upper) は upper 未満の最大値、nextValue(..., lower) は lower 以上の最小値。
// 候補がなければ prevValue / nextValue は nullopt を返す。
// 使いどころ: 更新のない配列で、部分区間の順位・値域個数・直前直後の値を多数問い合わせる場合。
// 具体例: A={3, -1, 4, 1} なら kth(0, 4, 1)=1、rangeFreq(0, 4, 0, 4)=2。
// kth の k は小さい方から k 番目の 0-indexed。元配列の位置を返す関数ではない。
template<class T>
struct WaveletMatrix {
    using U = make_unsigned_t<T>;
    static constexpr int B = numeric_limits<U>::digits;
    int N;
    vector<int> mid;
    vector<vector<int>> bit;

    // 符号付き T の大小順を unsigned の辞書順へ写す。
    U encode(T x) const {
        U u = (U)x;
        if constexpr (is_signed_v<T>) u ^= U(1) << (B - 1);
        return u;
    }
    // encode された unsigned 値を元の T へ戻す。
    T decode(U x) const {
        if constexpr (is_signed_v<T>) x ^= U(1) << (B - 1);
        return (T)x;
    }
    // 静的配列 A を各 bit で安定分割して構築する。
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
    // 半開区間 [l, r) に値 x が現れる個数を返す。
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
    // 半開区間 [l, r) を昇順に並べた 0-indexed k 番目の値を返す。
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
    // 半開区間 [l, r) にある x 未満の値の個数を返す。
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
    // 添字 [l, r)、値域 [low, high) の要素数を返す。
    int rangeFreq(int l, int r, T low, T high) const {
        return lessThan(l, r, high) - lessThan(l, r, low);
    }
    // [l, r) にある upper 未満の最大値を返す。存在しなければ nullopt。
    optional<T> prevValue(int l, int r, T upper) const {
        int k = lessThan(l, r, upper);
        if (k == 0) return nullopt;
        return kth(l, r, k - 1);
    }
    // [l, r) にある lower 以上の最小値を返す。存在しなければ nullopt。
    optional<T> nextValue(int l, int r, T lower) const {
        int k = lessThan(l, r, lower);
        if (k == r - l) return nullopt;
        return kth(l, r, k);
    }
};

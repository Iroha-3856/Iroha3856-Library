//体 T 上の Lagrange 補間。N 個の一般点から 1 点を O(N^2) 評価
template<class T>
T lagrangeEvaluate(const vector<T>& xs, const vector<T>& ys, T x) {
    int N = (int)xs.size();
    assert((int)ys.size() == N);
    for (int i = 0; i < N; i++) if (x == xs[i]) return ys[i];
    T ret = 0;
    for (int i = 0; i < N; i++) {
        T a = ys[i], b = 1;
        for (int j = 0; j < N; j++) if (i != j) {
            a *= x - xs[j];
            b *= xs[i] - xs[j];
        }
        ret += a / b;
    }
    return ret;
}

//ys[i] = f(i), deg(f) < N のとき f(x) を O(N) で評価
template<class T>
T lagrangeConsecutive(const vector<T>& ys, T x) {
    int N = (int)ys.size();
    if (N == 0) return T(0);
    for (int i = 0; i < N; i++) if (x == T(i)) return ys[i];
    vector<T> prefix(N + 1, T(1)), suffix(N + 1, T(1));
    for (int i = 0; i < N; i++) prefix[i + 1] = prefix[i] * (x - T(i));
    for (int i = N - 1; i >= 0; i--) suffix[i] = suffix[i + 1] * (x - T(i));
    vector<T> invFact(N, T(1));
    for (int i = 1; i < N; i++) invFact[i] = invFact[i - 1] / T(i);
    T ret = 0;
    for (int i = 0; i < N; i++) {
        T term = ys[i] * prefix[i] * suffix[i + 1] * invFact[i] * invFact[N - 1 - i];
        if ((N - 1 - i) & 1) ret -= term;
        else ret += term;
    }
    return ret;
}

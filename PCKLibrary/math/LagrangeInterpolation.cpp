// 体 T 上の Lagrange 補間。N 個の一般点から 1 点を O(N^2) 評価
// 使いどころ: 次数 N 未満の多項式について N 個の値だけが分かり、別の一点での値が必要な場合。
// 具体例: (0, 1), (1, 3) を通る一次式は 2x+1 なので、x=5 で lagrangeEvaluate(...)=11。
// 使い方:
// T y = lagrangeEvaluate(xs, ys, x); で相異なる標本点 (xs[i], ys[i]) を通る次数<Nの値を得る。
// xs[i]=i が連続するときは lagrangeConsecutive(ys, x) を使えば O(N)。
// T は modint や有理数など除算可能な体。合成数 mod で分母が非可逆になる型は使えない。
// lagrangeConsecutive では T(0),...,T(N-1) が相異なること。mod p なら N<=p が必要。
template<class T>
// 相異なる点 (xs[i], ys[i]) を通る次数 N 未満の多項式を x で評価する。
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

// ys[i] = f(i), deg(f) < N のとき f(x) を O(N) で評価
template<class T>
// f(i)=ys[i] が与えられた次数 N 未満の多項式を x で O(N) 評価する。
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

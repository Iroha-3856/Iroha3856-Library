// 行列積と非負整数乗。積 O(HWK)、N 次正方行列の n 乗 O(N^3 log n)。
// 使いどころ: 線形漸化式や固定サイズの状態遷移を、非常に大きい回数だけ繰り返す場合。
// 具体例: fib={{1, 1}, {1, 0}} なら matrixPower(fib, n)[0][1] は Fibonacci 数 F_n。
// 使い方:
// Matrix<T> A(H, vector<T>(W)); と通常の二次元 vector として作る。
// matrixProduct(A, B) は A の列数と B の行数が一致する行列積、matrixPower(A, n) は正方行列の A^n。
// T の +, *, 0, 1 を使うため、整数・modint のどちらでもよい。n==0 は単位行列を返す。
// Matrix<ll> fib={{1, 1}, {1, 0}}; Matrix<ll> p=matrixPower(fib, n); のように使う。
template<class T>
using Matrix = vector<vector<T>>;

// 行列積 O(HWK)
template<class T>
// 行列 A と B の積を返す。A の列数と B の行数が一致すること。
Matrix<T> matrixProduct(const Matrix<T>& A, const Matrix<T>& B) {
    int H = (int)A.size(), K = (int)B.size(), W = (int)B[0].size();
    Matrix<T> ret(H, vector<T>(W));
    for (int i = 0; i < H; i++) {
        for (int k = 0; k < K; k++) {
            for (int j = 0; j < W; j++) ret[i][j] += A[i][k] * B[k][j];
        }
    }
    return ret;
}

// 正方行列 A の n 乗。O(N^3 log n)
template<class T>
// 正方行列 A の非負整数 n 乗を二分累乗で返す。
Matrix<T> matrixPower(Matrix<T> A, long long n) {
    int N = (int)A.size();
    Matrix<T> ret(N, vector<T>(N));
    for (int i = 0; i < N; i++) ret[i][i] = 1;
    while (n) {
        if (n & 1) ret = matrixProduct(ret, A);
        A = matrixProduct(A, A);
        n >>= 1;
    }
    return ret;
}

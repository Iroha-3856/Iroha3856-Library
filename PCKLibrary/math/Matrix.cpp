template<class T>
using Matrix = vector<vector<T>>;

//行列積 O(HWK)
template<class T>
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

//正方行列 A の n 乗。O(N^3 log n)
template<class T>
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

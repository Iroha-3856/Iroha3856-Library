//AND / OR / XOR convolution 用 Fast Walsh-Hadamard Transform。O(N log N)
//N は二冪。inverse=true で逆変換する
enum class BitwiseConvolution { And, Or, Xor };

template<class T>
void fwt(vector<T>& A, BitwiseConvolution type, bool inverse = false) {
    int N = (int)A.size();
    assert(N > 0 and (N & (N - 1)) == 0);
    for (int len = 1; len < N; len <<= 1) {
        for (int l = 0; l < N; l += 2 * len) for (int i = 0; i < len; i++) {
            T x = A[l + i], y = A[l + len + i];
            if (type == BitwiseConvolution::Xor) {
                A[l + i] = x + y;
                A[l + len + i] = x - y;
            } else if (type == BitwiseConvolution::Or) {
                A[l + len + i] = inverse ? y - x : y + x;
            } else {
                A[l + i] = inverse ? x - y : x + y;
            }
        }
    }
    if (type == BitwiseConvolution::Xor and inverse) for (T& x : A) x /= T(N);
}

template<class T>
vector<T> bitwiseConvolution(vector<T> A, vector<T> B, BitwiseConvolution type) {
    assert(A.size() == B.size());
    fwt(A, type); fwt(B, type);
    for (int i = 0; i < (int)A.size(); i++) A[i] *= B[i];
    fwt(A, type, true);
    return A;
}

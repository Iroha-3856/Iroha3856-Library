// AND / OR / XOR convolution 用 Fast Walsh-Hadamard Transform。O(N log N)
// N は二冪。inverse=true で逆変換する
// 使いどころ: 添字の和ではなく、bitwise AND / OR / XOR で添字を合成する畳み込み。
// 具体例: A={1, 2}, B={3, 4} の XOR 畳み込みは {11, 10}。C[0]=1*3+2*4 である。
// 使い方:
// auto C = bitwiseConvolution(A, B, BitwiseConvolution::Xor); のように種類を選ぶ。
// C[k] は指定演算で i op j == k となる全 A[i]*B[j] の和。A, B は同じ二冪長にする。
// fwt(A, type) と fwt(A, type, true) を直接使う場合、XOR 逆変換で N による除算が必要な体を使う。
enum class BitwiseConvolution { And, Or, Xor };

template<class T>
// 配列 A を AND/OR/XOR 畳み込み用に正変換または逆変換する。
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
// 指定した bitwise 演算を添字に使う A と B の畳み込みを返す。
vector<T> bitwiseConvolution(vector<T> A, vector<T> B, BitwiseConvolution type) {
    assert(A.size() == B.size());
    fwt(A, type); fwt(B, type);
    for (int i = 0; i < (int)A.size(); i++) A[i] *= B[i];
    fwt(A, type, true);
    return A;
}

/*
商列挙
返り値に含まれる {l, r, x} は、l <= i < r について、N/i = x であることを示す。
返り値は l の昇順 = x の降順
x = 0 は返り値に含まれないことに注意
verify: https://judge.yosupo.jp/submission/364489
        https://atcoder.jp/contests/abc239/submissions/74721536
*/
template<class T> 
vector<tuple<T, T, T>> enumerateQuotients(T N) {
    vector<tuple<T, T, T>> ret;
    T M;
    for (M = 1; M * M <= N; M++) {
        ret.push_back({M, M + 1, N / M});
    }
    for (int i = M; i >= 1; i--) {
        T L = N / (i + 1) + 1, R = N / i + 1;
        if (L < R and get<1>(ret.back()) <= L) ret.push_back({L, R, i});
    }
    return ret;
}
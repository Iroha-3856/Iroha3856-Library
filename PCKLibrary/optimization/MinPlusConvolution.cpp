//依存: optimization/MonotoneMinima.cpp
//A が離散凸なら arbitrary B との min-plus convolution を O((N+M) log(N+M))
template<class T>
vector<T> minPlusConvolutionConvex(const vector<T>& A, const vector<T>& B, T infinity) {
    if (A.empty() or B.empty()) return {};
    int N = (int)A.size(), M = (int)B.size(), H = N + M - 1;
    auto get = [&](int k, int j) {
        int i = k - j;
        return 0 <= i and i < N and 0 <= j and j < M ? A[i] + B[j] : infinity;
    };
    vector<pair<int, T>> row = monotoneMinima<T>(H, M, get);
    vector<T> ret(H);
    for (int i = 0; i < H; i++) ret[i] = row[i].second;
    return ret;
}

//両方が離散凸なら差分列の merge で O(N + M)
template<class T>
vector<T> minPlusConvolutionConvexConvex(const vector<T>& A, const vector<T>& B) {
    if (A.empty() or B.empty()) return {};
    vector<T> ret = {A[0] + B[0]};
    int i = 1, j = 1;
    while (i < (int)A.size() or j < (int)B.size()) {
        if (j == (int)B.size() or (i < (int)A.size() and A[i] - A[i - 1] < B[j] - B[j - 1])) {
            ret.push_back(ret.back() + A[i] - A[i - 1]);
            i++;
        } else {
            ret.push_back(ret.back() + B[j] - B[j - 1]);
            j++;
        }
    }
    return ret;
}

//凹列と任意列。境界が小さい場合用の確実な O(NM) 版
template<class T>
vector<T> minPlusConvolutionConcave(const vector<T>& A, const vector<T>& B, T infinity) {
    if (A.empty() or B.empty()) return {};
    vector<T> ret(A.size() + B.size() - 1, infinity);
    for (int i = 0; i < (int)A.size(); i++) for (int j = 0; j < (int)B.size(); j++) {
        chmin(ret[i + j], A[i] + B[j]);
    }
    return ret;
}

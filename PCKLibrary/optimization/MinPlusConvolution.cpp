// 依存: optimization/MonotoneMinima.cpp
// A が離散凸なら arbitrary B との min-plus convolution を O((N+M) log(N+M))
// 使いどころ: 遷移が C[k]=min_i(A[i]+B[k-i]) の形で、少なくとも A が離散凸な場合。
// 具体例: A={0, 1}, B={0, 3} の min-plus convolution は {0, 1, 4}。
// 使い方:
// C[k]=min_{i+j=k}(A[i]+B[j]) を求める。空入力なら空。
// A が離散凸なら minPlusConvolutionConvex(A, B, INF)、両方が凸なら ConvexConvex(A, B)。
// 離散凸とは隣接差 A[i+1]-A[i] が非減少。ConvexConvex は差分 merge で O(N+M)。
// A が離散凹な minPlusConvolutionConcave は一般には高速化せず O(NM) の安全な実装。
template<class T>
// 離散凸列 A と任意列 B の min-plus convolution を返す。
vector<T> minPlusConvolutionConvex(const vector<T>& A, const vector<T>& B, T infinity) {
    if (A.empty() or B.empty()) return {};
    int N = (int)A.size(), M = (int)B.size(), H = N + M - 1;
    // 出力添字 k と B 側添字 j から、対応する A[k-j]+B[j] を返す。
    auto get = [&](int k, int j) {
        int i = k - j;
        return 0 <= i and i < N and 0 <= j and j < M ? A[i] + B[j] : infinity;
    };
    vector<pair<int, T>> row = monotoneMinima<T>(H, M, get);
    vector<T> ret(H);
    for (int i = 0; i < H; i++) ret[i] = row[i].second;
    return ret;
}

// 両方が離散凸なら差分列の merge で O(N + M)
template<class T>
// 二つの離散凸列の差分を merge し、線形時間で min-plus convolution を返す。
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

// 凹列と任意列。境界が小さい場合用の確実な O(NM) 版
template<class T>
// 離散凹列 A と任意列 B の min-plus convolution を愚直に返す。
vector<T> minPlusConvolutionConcave(const vector<T>& A, const vector<T>& B, T infinity) {
    if (A.empty() or B.empty()) return {};
    vector<T> ret(A.size() + B.size() - 1, infinity);
    for (int i = 0; i < (int)A.size(); i++) for (int j = 0; j < (int)B.size(); j++) {
        chmin(ret[i + j], A[i] + B[j]);
    }
    return ret;
}

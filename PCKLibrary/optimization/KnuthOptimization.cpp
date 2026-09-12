// 区間 DP: dp[l][r] = min_k(dp[l][k] + dp[k][r]) + cost(l, r)
// 四辺形不等式などにより opt[l][r-1] <= opt[l][r] <= opt[l+1][r] のとき O(N^2)
// 使いどころ: 区間の最終分割を選ぶ DP で、最適分割点が隣接区間の二点間に挟まる場合。
// 具体例: 重み {1, 2, 3} の最適マージで cost(l, r)=区間和とすると dp[0][3]=9。
// 使い方:
// auto [dp, opt] = knuthOptimization<T>(N, INF, cost); で区間 [0, N) の全 DP を求める。
// 空区間と長さ1は 0、長さ2以上では l<k<r を試す定義。答えは dp[0][N]。
// cost(l, r) は分割点 k に依存しない区間追加コストで、opt の包含単調性を満たす必要がある。
// 長さ2以上の各区間に有限な最適遷移が存在し、dp+dp+cost の計算が T の範囲内に収まること。
// opt[l][r] を再帰的に辿れば最適分割を復元できる。
template<class T, class Cost>
// Knuth の opt 包含単調性を使い、全区間の dp と最適分割点を返す。
pair<vector<vector<T>>, vector<vector<int>>> knuthOptimization(int N, T infinity, const Cost& cost) {
    vector<vector<T>> dp(N + 1, vector<T>(N + 1, infinity));
    vector<vector<int>> opt(N + 1, vector<int>(N + 1));
    for (int i = 0; i <= N; i++) dp[i][i] = 0, opt[i][i] = i;
    for (int i = 0; i < N; i++) dp[i][i + 1] = 0, opt[i][i + 1] = i + 1;
    for (int len = 2; len <= N; len++) for (int l = 0; l + len <= N; l++) {
        int r = l + len;
        int from = max(l + 1, opt[l][r - 1]);
        int to = min(r - 1, opt[l + 1][r]);
        for (int k = from; k <= to; k++) {
            T x = dp[l][k] + dp[k][r] + cost(l, r);
            if (x < dp[l][r]) dp[l][r] = x, opt[l][r] = k;
        }
    }
    return {dp, opt};
}

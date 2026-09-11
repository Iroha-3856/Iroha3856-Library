//区間 DP: dp[l][r] = min_k(dp[l][k] + dp[k][r]) + cost(l, r)
//四辺形不等式などにより opt[l][r-1] <= opt[l][r] <= opt[l+1][r] のとき O(N^2)
template<class T, class Cost>
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

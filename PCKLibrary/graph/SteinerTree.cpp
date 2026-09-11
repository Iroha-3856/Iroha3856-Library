//非負辺重みの無向グラフで、指定頂点を結ぶ Steiner Tree の最小コスト
//K 個の terminal に対して O(3^K N + 2^K (N + M) log N)、解なしは INF
ll steinerTree(const vector<vector<pair<int, ll>>>& G, const vector<int>& terminal) {
    int N = (int)G.size(), K = (int)terminal.size(), M = 1 << K;
    if (K == 0) return 0;
    vector<vector<ll>> dp(M, vector<ll>(N, INF));
    for (int i = 0; i < K; i++) dp[1 << i][terminal[i]] = 0;
    for (int s = 1; s < M; s++) {
        for (int a = (s - 1) & s; a; a = (a - 1) & s) {
            int b = s ^ a;
            if (a > b) continue;
            for (int v = 0; v < N; v++) {
                if (dp[a][v] == INF or dp[b][v] == INF) continue;
                chmin(dp[s][v], dp[a][v] + dp[b][v]);
            }
        }
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> que;
        for (int v = 0; v < N; v++) if (dp[s][v] != INF) que.push({dp[s][v], v});
        while (!que.empty()) {
            auto [d, v] = que.top(); que.pop();
            if (dp[s][v] != d) continue;
            for (auto [to, cost] : G[v]) if (dp[s][v] <= INF - cost) {
                if (chmin(dp[s][to], d + cost)) que.push({dp[s][to], to});
            }
        }
    }
    return *min_element(dp.back().begin(), dp.back().end());
}

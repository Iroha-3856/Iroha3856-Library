//到達不能は INF、負閉路を経由して到達できる頂点は -INF。O(VE)
vector<ll> bellmanFord(int N, const vector<tuple<int, int, ll>>& edges, int start) {
    vector<ll> dist(N, INF);
    dist[start] = 0;
    for (int i = 0; i < N - 1; i++) {
        bool update = false;
        for (auto [from, to, cost] : edges) {
            if (dist[from] == INF) continue;
            update |= chmin(dist[to], dist[from] + cost);
        }
        if (!update) break;
    }
    //N - 1 回後も緩和できる頂点は負閉路の影響下にある
    for (int i = 0; i < N; i++) {
        for (auto [from, to, cost] : edges) {
            if (dist[from] == INF) continue;
            if (dist[from] == -INF or dist[to] > dist[from] + cost) dist[to] = -INF;
        }
    }
    return dist;
}

// 到達不能は INF、負閉路を経由して到達できる頂点は -INF。O(VE)
// 使い方:
// edges に有向辺 {from, to, cost} を並べ、vector<ll> dist = bellmanFord(N, edges, s);
// dist[v] == INF は到達不能、dist[v] == -INF は s から到達可能な負閉路の影響下を表す。
// 無向辺は両方向を追加する。INF と辺コストを足しても ll を超えないようにする。
// 使いどころ: 負辺がある最短路、または始点から影響を受ける負閉路の頂点を特定したい場合。
// 具体例: 0->1 のコスト2、1->2 のコスト-5なら、始点0からの距離は {0, 2, -3}。
// -INF は「距離が非常に小さい」のではなく、負閉路を何周でもできて最短値が定まらないことを表す。
// start からの最短距離を返し、到達可能な負閉路の影響を -INF として伝播する。
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
    // N - 1 回後も緩和できる頂点は負閉路の影響下にある
    for (int i = 0; i < N; i++) {
        for (auto [from, to, cost] : edges) {
            if (dist[from] == INF) continue;
            if (dist[from] == -INF or dist[to] > dist[from] + cost) dist[to] = -INF;
        }
    }
    return dist;
}

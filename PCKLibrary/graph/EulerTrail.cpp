// 有向・無向グラフの Euler 路。O(N + M)
// 成功時は M+1 頂点と M 個の edge ID、存在しなければ nullopt
// 使い方:
// 辺を入力順の {u, v} として directedEulerTrail(N, edges) または
// undirectedEulerTrail(N, edges) へ渡す。返り値が nullopt なら全辺を一度ずつ通る路はない。
// result.vertices が頂点列、result.edgeIds[i] が vertices[i] から次へ使った入力辺番号。
// 空辺集合では N>0 なら頂点 0 のみ、N==0 なら番兵 -1 の頂点列を返す。
// 使いどころ: 全辺をちょうど一度使う経路、文字列復元、de Bruijn 型の辺列復元。
// 具体例: 無向辺 {0-1, 1-2} なら vertices={0, 1, 2}、edgeIds は対応する入力辺番号列。
// 頂点次数の条件だけでなく、辺を持つ頂点が一つの連結成分にあることも内部で確認する。
struct EulerTrailResult {
    vector<int> vertices, edgeIds;
};

// 有向辺列 edges をすべて一度ずつ使う Euler 路を返す。存在しなければ nullopt。
optional<EulerTrailResult> directedEulerTrail(int N, const vector<pair<int, int>>& edges) {
    vector<vector<pair<int, int>>> G(N);
    vector<int> in(N), out(N), it(N);
    for (int id = 0; id < (int)edges.size(); id++) {
        auto [u, v] = edges[id];
        G[u].push_back({v, id}); out[u]++; in[v]++;
    }
    int start = -1, plus = 0, minus = 0;
    for (int v = 0; v < N; v++) {
        if (out[v] - in[v] == 1) start = v, plus++;
        else if (in[v] - out[v] == 1) minus++;
        else if (in[v] != out[v]) return nullopt;
        if (start == -1 and out[v]) start = v;
    }
    if (!((plus == 1 and minus == 1) or (plus == 0 and minus == 0))) return nullopt;
    if (edges.empty()) return EulerTrailResult{{N ? 0 : -1}, {}};
    vector<int> stV = {start}, stE = {-1}, vs, es;
    while (!stV.empty()) {
        int v = stV.back();
        if (it[v] < (int)G[v].size()) {
            auto [to, id] = G[v][it[v]++];
            stV.push_back(to); stE.push_back(id);
        } else {
            vs.push_back(v);
            if (stE.back() != -1) es.push_back(stE.back());
            stV.pop_back(); stE.pop_back();
        }
    }
    if ((int)es.size() != (int)edges.size()) return nullopt;
    reverse(vs.begin(), vs.end()); reverse(es.begin(), es.end());
    return EulerTrailResult{vs, es};
}

// 無向辺列 edges をすべて一度ずつ使う Euler 路を返す。存在しなければ nullopt。
optional<EulerTrailResult> undirectedEulerTrail(int N, const vector<pair<int, int>>& edges) {
    vector<vector<pair<int, int>>> G(N);
    vector<int> degree(N), it(N);
    for (int id = 0; id < (int)edges.size(); id++) {
        auto [u, v] = edges[id];
        G[u].push_back({v, id}); G[v].push_back({u, id});
        degree[u]++; degree[v]++;
    }
    vector<int> odd;
    int start = -1;
    for (int v = 0; v < N; v++) {
        if (degree[v] & 1) odd.push_back(v);
        if (degree[v]) start = v;
    }
    if (!(odd.empty() or odd.size() == 2)) return nullopt;
    if (!odd.empty()) start = odd[0];
    if (edges.empty()) return EulerTrailResult{{N ? 0 : -1}, {}};
    vector<bool> used(edges.size());
    vector<int> stV = {start}, stE = {-1}, vs, es;
    while (!stV.empty()) {
        int v = stV.back();
        while (it[v] < (int)G[v].size() and used[G[v][it[v]].second]) it[v]++;
        if (it[v] < (int)G[v].size()) {
            auto [to, id] = G[v][it[v]++];
            used[id] = true;
            stV.push_back(to); stE.push_back(id);
        } else {
            vs.push_back(v);
            if (stE.back() != -1) es.push_back(stE.back());
            stV.pop_back(); stE.pop_back();
        }
    }
    if ((int)es.size() != (int)edges.size()) return nullopt;
    reverse(vs.begin(), vs.end()); reverse(es.begin(), es.end());
    return EulerTrailResult{vs, es};
}

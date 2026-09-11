//有向・無向グラフの Euler 路。O(N + M)
//成功時は M+1 頂点と M 個の edge ID、存在しなければ nullopt
struct EulerTrailResult {
    vector<int> vertices, edgeIds;
};

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

//無向グラフの橋・関節点。多重辺に対応、O(N + M)
//bridge は edge ID、articulation は頂点番号。自己ループは橋にならない
struct LowLink {
    int N;
    vector<pair<int, int>> edges;
    vector<vector<pair<int, int>>> G;
    vector<int> order, low, bridge, articulation;
    int timer = 0;

    LowLink(int n) : N(n), G(n) {}
    int addEdge(int u, int v) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        G[u].push_back({v, id});
        G[v].push_back({u, id});
        return id;
    }
    void dfs(int v, int parentEdge) {
        order[v] = low[v] = timer++;
        int children = 0;
        bool cut = false;
        for (auto [to, id] : G[v]) {
            if (id == parentEdge) continue;
            if (order[to] == -1) {
                children++;
                dfs(to, id);
                low[v] = min(low[v], low[to]);
                if (order[v] < low[to]) bridge.push_back(id);
                if (parentEdge != -1 and order[v] <= low[to]) cut = true;
            } else {
                low[v] = min(low[v], order[to]);
            }
        }
        if (parentEdge == -1 and children >= 2) cut = true;
        if (cut) articulation.push_back(v);
    }
    void build() {
        order.assign(N, -1); low.resize(N);
        bridge.clear(); articulation.clear(); timer = 0;
        for (int v = 0; v < N; v++) if (order[v] == -1) dfs(v, -1);
    }
};

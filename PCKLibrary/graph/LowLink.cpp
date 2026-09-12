// 無向グラフの橋・関節点。多重辺に対応、O(N + M)
// bridge は edge ID、articulation は頂点番号。自己ループは橋にならない
// 使い方:
// LowLink graph(N); int id = graph.addEdge(u, v); として全無向辺を追加し graph.build();
// graph.bridge には addEdge の返した辺番号、graph.articulation には関節点が入る。
// order/low も参照できる。入力が非連結でも全成分を処理する。
// 使いどころ: 一辺を消すと非連結になる橋、一頂点を消すと成分数が増える関節点の列挙。
// 具体例: 無向三角形 0-1-2-0 に橋・関節点はなく、辺 2-3 を足すとその辺が橋、頂点2が関節点になる。
// low[v] は v の DFS 部分木から後退辺で到達できる最小 order。bridge は端点対でなく edge ID。
// 二辺連結成分や block-cut tree まで必要なら BiconnectedComponents を使う。
struct LowLink {
    int N;
    vector<pair<int, int>> edges;
    vector<vector<pair<int, int>>> G;
    vector<int> order, low, bridge, articulation;
    int timer = 0;

    // 頂点数 n の空の無向グラフを作る。
    LowLink(int n) : N(n), G(n) {}
    // 無向辺 u-v を追加して edge ID を返す。多重辺は異なる ID になる。
    int addEdge(int u, int v) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        G[u].push_back({v, id});
        G[v].push_back({u, id});
        return id;
    }
    // DFS 部分木の order/low を求め、橋と関節点を記録する内部関数。
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
    // 非連結成分を含むグラフ全体に low-link を実行し、結果配列を作り直す。
    void build() {
        order.assign(N, -1); low.resize(N);
        bridge.clear(); articulation.clear(); timer = 0;
        for (int v = 0; v < N; v++) if (order[v] == -1) dfs(v, -1);
    }
};

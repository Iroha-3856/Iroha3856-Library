//無向グラフの二辺連結成分・二重頂点連結成分。多重辺対応、O(N + M)
//twoEdgeComponent は頂点の成分番号、vertexBlocks は各 block の頂点集合
struct BiconnectedComponents {
    int N, timer = 0;
    vector<pair<int, int>> edges;
    vector<vector<pair<int, int>>> G;
    vector<int> order, low, edgeStack, bridge, articulation, twoEdgeComponent;
    vector<vector<int>> vertexBlocks;

    BiconnectedComponents(int n) : N(n), G(n) {}
    int addEdge(int u, int v) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        G[u].push_back({v, id}); G[v].push_back({u, id});
        return id;
    }
    void makeBlock(int stopEdge) {
        vector<int> vs;
        while (true) {
            int id = edgeStack.back(); edgeStack.pop_back();
            vs.push_back(edges[id].first); vs.push_back(edges[id].second);
            if (id == stopEdge) break;
        }
        sort(vs.begin(), vs.end());
        vs.erase(unique(vs.begin(), vs.end()), vs.end());
        vertexBlocks.push_back(vs);
    }
    void dfs(int v, int parentEdge) {
        order[v] = low[v] = timer++;
        int children = 0;
        bool cut = false;
        for (auto [to, id] : G[v]) {
            if (id == parentEdge) continue;
            if (order[to] == -1) {
                children++;
                edgeStack.push_back(id);
                dfs(to, id);
                low[v] = min(low[v], low[to]);
                if (order[v] < low[to]) bridge.push_back(id);
                if (order[v] <= low[to]) {
                    makeBlock(id);
                    if (parentEdge != -1) cut = true;
                }
            } else if (order[to] < order[v]) {
                edgeStack.push_back(id);
                low[v] = min(low[v], order[to]);
            }
        }
        if (parentEdge == -1 and children >= 2) cut = true;
        if (cut) articulation.push_back(v);
        if (G[v].empty()) vertexBlocks.push_back({v});
    }
    void build() {
        order.assign(N, -1); low.resize(N); timer = 0;
        bridge.clear(); articulation.clear(); vertexBlocks.clear(); edgeStack.clear();
        for (int v = 0; v < N; v++) if (order[v] == -1) dfs(v, -1);
        vector<bool> isBridge(edges.size());
        for (int id : bridge) isBridge[id] = true;
        twoEdgeComponent.assign(N, -1);
        int C = 0;
        for (int s = 0; s < N; s++) if (twoEdgeComponent[s] == -1) {
            vector<int> st = {s};
            twoEdgeComponent[s] = C;
            while (!st.empty()) {
                int v = st.back(); st.pop_back();
                for (auto [to, id] : G[v]) if (!isBridge[id] and twoEdgeComponent[to] == -1) {
                    twoEdgeComponent[to] = C;
                    st.push_back(to);
                }
            }
            C++;
        }
    }
    vector<vector<int>> bridgeTree() const {
        int C = *max_element(twoEdgeComponent.begin(), twoEdgeComponent.end()) + 1;
        vector<vector<int>> T(C);
        for (int id : bridge) {
            auto [u, v] = edges[id];
            u = twoEdgeComponent[u]; v = twoEdgeComponent[v];
            T[u].push_back(v); T[v].push_back(u);
        }
        return T;
    }
    vector<vector<int>> blockCutTree() const {
        int B = (int)vertexBlocks.size();
        vector<int> cutId(N, -1);
        for (int i = 0; i < (int)articulation.size(); i++) cutId[articulation[i]] = B + i;
        vector<vector<int>> T(B + articulation.size());
        for (int b = 0; b < B; b++) for (int v : vertexBlocks[b]) if (cutId[v] != -1) {
            T[b].push_back(cutId[v]); T[cutId[v]].push_back(b);
        }
        return T;
    }
};

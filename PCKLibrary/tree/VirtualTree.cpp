//依存: tree/LowestCommonAncestor.cpp
//指定頂点と必要な LCA だけからなる Auxiliary / Virtual Tree。構築 O(K log K log N)
//返す辺は元の木の頂点番号で親から子。vertices は Euler Tour 順
struct VirtualTreeResult {
    vector<int> vertices;
    vector<pair<int, int>> edges;
};

struct VirtualTree {
    int N, timer = 0;
    vector<vector<int>> G;
    vector<int> in, out;
    LowestCommonAncestor lca;

    VirtualTree(const vector<vector<int>>& g, int root = 0) : N((int)g.size()), G(g),
        in(N), out(N), lca(g, root) {
        dfs(root, -1);
    }
    void dfs(int v, int p) {
        in[v] = timer++;
        for (int to : G[v]) if (to != p) dfs(to, v);
        out[v] = timer;
    }
    bool ancestor(int u, int v) const { return in[u] <= in[v] and out[v] <= out[u]; }
    VirtualTreeResult build(vector<int> V) {
        if (V.empty()) return {};
        sort(V.begin(), V.end(), [&](int a, int b) { return in[a] < in[b]; });
        int K = (int)V.size();
        for (int i = 0; i + 1 < K; i++) V.push_back(lca.lca(V[i], V[i + 1]));
        sort(V.begin(), V.end(), [&](int a, int b) { return in[a] < in[b]; });
        V.erase(unique(V.begin(), V.end()), V.end());
        vector<pair<int, int>> edges;
        vector<int> st;
        for (int v : V) {
            while (!st.empty() and !ancestor(st.back(), v)) st.pop_back();
            if (!st.empty()) edges.push_back({st.back(), v});
            st.push_back(v);
        }
        return {V, edges};
    }
};

//依存: data_structure/SparseTable.cpp
//Euler Tour + Sparse Table。構築 O(NlogN)、LCA O(1)
pair<int, int> lcaMin(pair<int, int> a, pair<int, int> b) {
    return min(a, b);
}

struct LowestCommonAncestorSparseTable {
    vector<vector<int>> G;
    vector<int> depth, first;
    vector<pair<int, int>> euler;
    SparseTable<pair<int, int>, lcaMin> table;

    LowestCommonAncestorSparseTable(const vector<vector<int>>& g, int root = 0)
        : G(g), depth(g.size()), first(g.size()) {
        dfs(root, -1, 0);
        table.build(euler);
    }
    void dfs(int v, int parent, int d) {
        depth[v] = d;
        first[v] = (int)euler.size();
        euler.push_back({d, v});
        for (int to : G[v]) {
            if (to == parent) continue;
            dfs(to, v, d + 1);
            euler.push_back({d, v});
        }
    }
    int lca(int u, int v) const {
        int l = first[u], r = first[v];
        if (l > r) swap(l, r);
        return table.query(l, r + 1).second;
    }
    int dist(int u, int v) const {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
};

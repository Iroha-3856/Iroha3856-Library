// 依存: data_structure/SparseTable.cpp
// Euler Tour + Sparse Table。構築 O(NlogN)、LCA O(1)
// 使い方:
// 依存先を先に置き、LowestCommonAncestorSparseTable tree(G, root); と構築する。
// tree.lca(u, v) と tree.dist(u, v) を使う。静的な木で LCA を多数問い合わせる場合に向く。
// G は空でない連結な無向木、root は有効な頂点番号とする。辺重みは扱わず、dist は辺数を返す。
// 使いどころ: 木が不変で、O(N log N) の前計算後に LCA を非常に多く O(1) で求めたい場合。
// 具体例: 辺 0-1, 1-2, 1-3 の根0の木では lca(2, 3)=1、dist(2, 3)=2。
// Euler Tour には頂点へ入る時と子から戻る時を記録し、二頂点の初出間で深さ最小を取る。
// jump やパス上 k 番目も必要なら、API が多いダブリング版を使う。
// {深さ, 頂点} のうち浅い方を返す Sparse Table 用演算。
pair<int, int> lcaMin(pair<int, int> a, pair<int, int> b) {
    return min(a, b);
}

struct LowestCommonAncestorSparseTable {
    vector<vector<int>> G;
    vector<int> depth, first;
    vector<pair<int, int>> euler;
    SparseTable<pair<int, int>, lcaMin> table;

    // g を root から Euler Tour し、深さ RMQ の Sparse Table を構築する。
    LowestCommonAncestorSparseTable(const vector<vector<int>>& g, int root = 0)
        : G(g), depth(g.size()), first(g.size()) {
        assert(!G.empty() and 0 <= root and root < (int)G.size());
        dfs(root, -1, 0);
        table.build(euler);
    }
    // Euler Tour 列と各頂点の初出位置を再帰的に構築する。
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
    // u, v の初出位置間で深さ最小の頂点、すなわち LCA を O(1) で返す。
    int lca(int u, int v) const {
        int l = first[u], r = first[v];
        if (l > r) swap(l, r);
        return table.query(l, r + 1).second;
    }
    // u-v パスの辺数を返す。
    int dist(int u, int v) const {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
};

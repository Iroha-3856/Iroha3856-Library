// 依存: tree/LowestCommonAncestor.cpp
// 指定頂点と必要な LCA だけからなる Auxiliary / Virtual Tree。構築 O(K log K log N)
// 返す辺は元の木の頂点番号で親から子。vertices は Euler Tour 順
// 使い方:
// 依存先を先に置き VirtualTree tree(G, root); を一度構築する。G は空でない連結な無向木。
// auto result = tree.build(queryVertices); とすると、必要な LCA を加えた頂点と親子辺を返す。
// result.edges の端点は圧縮後の添字でなく元の頂点番号。辺重みは元木上の距離から計算する。
// 入力頂点の重複は build 内で除去され、空集合なら空の結果を返す。
// 使いどころ: 一回の query に現れる K 頂点だけについて、元木上の関係を O(K) 規模へ圧縮する場合。
// 具体例: result.edges の {u, v} の重みは lca.dist(u, v) として元木上の距離を付ける。
// 元木の中間頂点は、指定頂点同士の分岐点となる LCA だけが追加され、それ以外は辺の長さへ畳み込む。
struct VirtualTreeResult {
    vector<int> vertices;
    vector<pair<int, int>> edges;
};

struct VirtualTree {
    int N, timer = 0;
    vector<vector<int>> G;
    vector<int> in, out;
    LowestCommonAncestor lca;

    // 元木 g を root で根付け、祖先判定用 Euler Tour と LCA を前計算する。
    VirtualTree(const vector<vector<int>>& g, int root = 0) : N((int)g.size()), G(g),
        in(N), out(N), lca(g, root) {
        assert(N > 0 and 0 <= root and root < N);
        dfs(root, -1);
    }
    // 各頂点の進入時刻 in と部分木終了時刻 out を求める。
    void dfs(int v, int p) {
        in[v] = timer++;
        for (int to : G[v]) if (to != p) dfs(to, v);
        out[v] = timer;
    }
    // u が v の祖先または u==v かを Euler Tour 区間で判定する。
    bool ancestor(int u, int v) const { return in[u] <= in[v] and out[v] <= out[u]; }
    // 指定頂点と必要な LCA だけを残し、元頂点番号による Virtual Tree を返す。
    VirtualTreeResult build(vector<int> V) {
        if (V.empty()) return {};
        // 隣接する必要頂点の LCA だけで閉包を作れるよう Euler Tour 順へ並べる。
        sort(V.begin(), V.end(), [&](int a, int b) { return in[a] < in[b]; });
        int K = (int)V.size();
        for (int i = 0; i + 1 < K; i++) V.push_back(lca.lca(V[i], V[i + 1]));
        // 追加した LCA を含めて再整列し、重複頂点を続く unique で除く。
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

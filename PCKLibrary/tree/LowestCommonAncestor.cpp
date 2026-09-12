// 構築 O(NlogN)、クエリ O(logN)
// 使い方:
// 無向木から LowestCommonAncestor tree(G, root); を作る。
// lca(u, v) は最近共通祖先、dist(u, v) は辺数、jump(v, k) は k 個上の祖先を返す。
// kthVertex(u, v, k) は u から v へのパス上で u を 0 番目とする頂点。範囲外は -1。
// G は空でない連結な無向木、root は有効な頂点番号とする。
// 使いどころ: 静的木で LCA、距離、祖先、パス上の位置を O(log N) で求める場合。
// 具体例: パス 2-1-3 なら lca(2, 3)=1、dist(2, 3)=2、kthVertex(2, 3, 1)=1。
// par[k][v] は v の 2^k 個上の祖先。辺重みは保持せず、dist は辺数である。
struct LowestCommonAncestor {
    int log;
    vector<vector<int>> par;
    vector<int> depth;

    // 無向木 G を root で根付け、2^k 個上の祖先表を構築する。
    LowestCommonAncestor(const vector<vector<int>>& G, int root = 0) {
        int N = (int)G.size();
        assert(N > 0 and 0 <= root and root < N);
        log = 1;
        while ((1 << log) < N) log++;
        par.assign(log, vector<int>(N, -1));
        depth.assign(N, -1);
        queue<int> Q;
        depth[root] = 0; Q.push(root);
        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            for (int to : G[v]) {
                if (to == par[0][v]) continue;
                par[0][to] = v;
                depth[to] = depth[v] + 1;
                Q.push(to);
            }
        }
        for (int k = 0; k + 1 < log; k++) {
            for (int v = 0; v < N; v++) {
                if (par[k][v] != -1) par[k + 1][v] = par[k][par[k][v]];
            }
        }
    }
    // v から k 辺だけ祖先へ移動した頂点を返す。根を越えれば -1。
    int jump(int v, int k) {
        for (int i = 0; i < log and v != -1; i++) if ((k >> i) & 1) v = par[i][v];
        return v;
    }
    // u と v の最近共通祖先を返す。
    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = jump(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int k = log - 1; k >= 0; k--) {
            if (par[k][u] != par[k][v]) u = par[k][u], v = par[k][v];
        }
        return par[0][u];
    }
    // u-v パスの辺数を返す。
    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
    // u-v パス上で u を 0 番目とする k 番目の頂点を返す。範囲外は -1。
    int kthVertex(int u, int v, int k) {
        int w = lca(u, v);
        int d = depth[u] + depth[v] - 2 * depth[w];
        if (k < 0 or k > d) return -1;
        if (k <= depth[u] - depth[w]) return jump(u, k);
        return jump(v, d - k);
    }
};

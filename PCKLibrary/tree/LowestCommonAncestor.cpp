//構築 O(NlogN)、クエリ O(logN)
struct LowestCommonAncestor {
    int log;
    vector<vector<int>> par;
    vector<int> depth;

    LowestCommonAncestor(const vector<vector<int>>& G, int root = 0) {
        int N = (int)G.size();
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
    int jump(int v, int k) {
        for (int i = 0; i < log and v != -1; i++) if ((k >> i) & 1) v = par[i][v];
        return v;
    }
    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = jump(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int k = log - 1; k >= 0; k--) {
            if (par[k][u] != par[k][v]) u = par[k][u], v = par[k][v];
        }
        return par[0][u];
    }
    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
    int kthVertex(int u, int v, int k) {
        int w = lca(u, v);
        int d = depth[u] + depth[v] - 2 * depth[w];
        if (k < 0 or k > d) return -1;
        if (k <= depth[u] - depth[w]) return jump(u, k);
        return jump(v, d - k);
    }
};

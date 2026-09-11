//Heavy-Light Decomposition。構築 O(N)、パス分解 O(log N)
//頂点・部分木は in の半開区間。path の reversed は r-1 から l へ読むことを表す
struct HeavyLightDecomposition {
    struct Segment {
        int l, r;
        bool reversed;
    };
    int N;
    vector<vector<int>> G;
    vector<int> parent, depth, sub, heavy, head, in, out, vertex;
    int timer = 0;

    HeavyLightDecomposition(const vector<vector<int>>& g, int root = 0) : N((int)g.size()), G(g),
        parent(N, -1), depth(N), sub(N), heavy(N, -1), head(N), in(N), out(N), vertex(N) {
        dfsSize(root, -1);
        dfsOrder(root, root);
    }
    void dfsSize(int v, int p) {
        parent[v] = p;
        sub[v] = 1;
        int best = 0;
        for (int to : G[v]) if (to != p) {
            depth[to] = depth[v] + 1;
            dfsSize(to, v);
            sub[v] += sub[to];
            if (best < sub[to]) best = sub[to], heavy[v] = to;
        }
    }
    void dfsOrder(int v, int h) {
        head[v] = h;
        in[v] = timer;
        vertex[timer++] = v;
        if (heavy[v] != -1) dfsOrder(heavy[v], h);
        for (int to : G[v]) if (to != parent[v] and to != heavy[v]) dfsOrder(to, to);
        out[v] = timer;
    }
    int lca(int u, int v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) u = parent[head[u]];
            else v = parent[head[v]];
        }
        return depth[u] < depth[v] ? u : v;
    }
    pair<int, int> subtree(int v, bool edge = false) const {
        return {in[v] + edge, out[v]};
    }
    vector<Segment> path(int u, int v, bool edge = false) const {
        vector<Segment> left, right;
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                left.push_back({in[head[u]], in[u] + 1, true});
                u = parent[head[u]];
            } else {
                right.push_back({in[head[v]], in[v] + 1, false});
                v = parent[head[v]];
            }
        }
        if (depth[u] > depth[v]) left.push_back({in[v] + edge, in[u] + 1, true});
        else right.push_back({in[u] + edge, in[v] + 1, false});
        reverse(right.begin(), right.end());
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }
};

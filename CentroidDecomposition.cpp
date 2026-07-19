struct CentroidDecomposition {
    int n;
    vector<bool> removed;
    vector<vector<int>> G;
    vector<int> sub;
    vector<vector<int>> centroid_tree;
    int root;

    CentroidDecomposition(int N) : n(N), removed(N, false), sub(N), G(N), centroid_tree(N) {}
    CentroidDecomposition(const vector<vector<int>>& g) : G(g) {
        n = (int)g.size();
        removed.resize((int)g.size(), false);
        sub.resize((int)g.size());
        centroid_tree.resize((int)g.size());
    }
    void add_edge(int u, int v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }
    int size(int v, int par) {
        sub[v] = 1;
        for (int to : G[v]) {
            if (to == par or removed[to]) continue;
            sub[v] += size(to, v);
        }
        return sub[v];
    }
    int centroid(int v, int par, int mid) {
        for (int to : G[v]) {
            if (to == par or removed[to]) continue;
            if (sub[to] > mid) return centroid(to, v, mid);
        }
        return v;
    }
    int build(int v) {
        int cent = centroid(v, -1, size(v, -1)/2);
        removed[cent] = true;
        for (int to : G[cent]) {
            if (removed[to]) continue;
            int t = build(to);
            assert(cent != t);
            centroid_tree[cent].push_back(t);
        }
        removed[cent] = false;
        return cent;
    }
    //treeは、各頂点からその重心分解木における子頂点のvector
    pair<int, vector<vector<int>>> run() {
        root = build(0);
        return {root, centroid_tree};
    }
};
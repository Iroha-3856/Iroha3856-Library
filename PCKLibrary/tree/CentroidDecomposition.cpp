//木の重心分解。O(N log N)
//parent は重心分解木の親、level は深さ、root は分解木の根
struct CentroidDecomposition {
    int N, root = -1;
    vector<vector<int>> G;
    vector<int> sub, parent, level;
    vector<bool> removed;

    CentroidDecomposition(const vector<vector<int>>& g) : N((int)g.size()), G(g), sub(N),
        parent(N, -1), level(N), removed(N) {
        if (N) root = build(0, -1, 0);
    }
    int dfsSize(int v, int p) {
        sub[v] = 1;
        for (int to : G[v]) if (to != p and !removed[to]) sub[v] += dfsSize(to, v);
        return sub[v];
    }
    int findCentroid(int v, int p, int total) {
        for (int to : G[v]) if (to != p and !removed[to] and sub[to] * 2 > total) {
            return findCentroid(to, v, total);
        }
        return v;
    }
    int build(int start, int p, int d) {
        int c = findCentroid(start, -1, dfsSize(start, -1));
        parent[c] = p;
        level[c] = d;
        removed[c] = true;
        for (int to : G[c]) if (!removed[to]) build(to, c, d + 1);
        return c;
    }
};

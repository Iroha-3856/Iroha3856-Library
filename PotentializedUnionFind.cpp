template<class T> struct PotentializedUnionFind {
    vector<int> par, siz;
    vector<T> potential;
    PotentializedUnionFind() : PotentializedUnionFind(0) {}
    PotentializedUnionFind(int n) {
        init(n);
    }
    void init(int n) {
        par.assign(n, -1);
        siz.assign(n, 1);
        potential.assign(n, 0);
    }
    int root(int x) {
        if (par[x]==-1) return x;
        int r = root(par[x]);
        potential[x] += potential[par[x]];
        return par[x] = r;
    }
    T getPotential(int x) {
        root(x);
        return potential[x];
    }
    bool same(int x, int y) {
        return root(x)==root(y);
    }
    //weight[y]-weight[x]=wとなるように辺を張る
    bool merge(int x, int y, T w) {
        w += getPotential(x); w -= getPotential(y);
        x = root(x); y = root(y);
        if (x == y) {
            if (getPotential(y) - getPotential(x) == w) return true;
            return false;
        }
        if (siz[x] < siz[y]) swap(x, y), w = -w;
        par[y] = x;
        potential[y] = w;
        return true;
    }
};
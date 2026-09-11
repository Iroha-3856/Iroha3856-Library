//weight[y] - weight[x] = w となるように併合する重み付き Union-Find
template<class T>
struct PotentializedUnionFind {
    vector<int> data;
    vector<T> potential;

    PotentializedUnionFind(int N) : data(N, -1), potential(N, 0) {}
    int root(int x) {
        if (data[x] < 0) return x;
        int p = data[x];
        data[x] = root(p);
        potential[x] += potential[p];
        return data[x];
    }
    T getPotential(int x) {
        root(x);
        return potential[x];
    }
    bool same(int x, int y) {
        return root(x) == root(y);
    }
    int size(int x) {
        return -data[root(x)];
    }
    //同じ連結成分なら、条件と矛盾しないかを返す
    bool merge(int x, int y, T w) {
        w += getPotential(x) - getPotential(y);
        x = root(x); y = root(y);
        if (x == y) return w == T(0);
        if (data[x] > data[y]) swap(x, y), w = -w;
        data[x] += data[y];
        data[y] = x;
        potential[y] = w;
        return true;
    }
    T diff(int x, int y) {
        assert(same(x, y));
        return getPotential(y) - getPotential(x);
    }
};

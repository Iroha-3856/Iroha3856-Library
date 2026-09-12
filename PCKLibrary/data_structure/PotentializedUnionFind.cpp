// weight[y] - weight[x] = w となるように併合する重み付き Union-Find
// 使い方:
// PotentializedUnionFind<ll> uf(N); uf.merge(x, y, w); で weight[y]-weight[x]=w を追加する。
// same(x, y) のとき uf.diff(x, y) がその差を返す。非連結頂点へ diff を呼ばないこと。
// merge は矛盾を判定しない。同じ成分なら false なので、必要なら既存の diff と照合する。
// 使いどころ: 頂点間の差分制約を追加しながら、二点間の相対値を問い合わせる場合。
// 具体例: merge(0, 1, 3), merge(1, 2, 4) の後は diff(0, 2)=7。
// T は加減算・零との比較ができる型。w の向きは常に weight[y]-weight[x]。
template<class T>
struct PotentializedUnionFind {
    vector<int> data;
    vector<T> potential;

    // N 個の独立した頂点を potential 0 で初期化する。
    PotentializedUnionFind(int N) : data(N, -1), potential(N, 0) {}
    // x の根を返し、経路圧縮と同時に potential[x]-potential[root] を更新する。
    int root(int x) {
        if (data[x] < 0) return x;
        int p = data[x];
        data[x] = root(p);
        potential[x] += potential[p];
        return data[x];
    }
    // potential[x]-potential[root(x)] を返す。
    T getPotential(int x) {
        root(x);
        return potential[x];
    }
    // x と y が同じ連結成分かを返す。
    bool same(int x, int y) {
        return root(x) == root(y);
    }
    // x を含む連結成分の頂点数を返す。
    int size(int x) {
        return -data[root(x)];
    }
    // potential[y]-potential[x]=w を追加する。同じ成分なら条件と矛盾しないかを返す。
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
    // potential[y]-potential[x] を返す。x と y は同じ成分でなければならない。
    T diff(int x, int y) {
        assert(same(x, y));
        return getPotential(y) - getPotential(x);
    }
};

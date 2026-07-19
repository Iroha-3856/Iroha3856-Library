
//Union-Find
struct UnionFind {
    int _n;
    vector<int> par, siz;
    int connected_components;
    UnionFind() : UnionFind(0) {}
    //n頂点0辺のグラフを作る
    UnionFind(int n) : _n(n) {
        par.resize(n, -1);
        siz.resize(n, 1);
        connected_components = n;
    }
    //頂点xの連結成分の代表をreturnする
    int leader(int x) {
        if (par[x]==-1) return x;
        return par[x] = leader(par[x]);
    }
    //頂点xと頂点yが同じ連結成分に属しているか判定する
    bool same(int x, int y) {
        return leader(x)==leader(y);
    }
    //頂点xと頂点yを結ぶ辺を作る
    bool merge(int x, int y) {
        x = leader(x); y = leader(y);
        if (x==y) return false;
        //siz[x] > siz[y]を前提とする
        if (siz[x] < siz[y]) swap(x, y);
        par[y] = x;
        siz[x]+=siz[y];
        connected_components--;
        return true;
    }
    //頂点xの連結成分の頂点数をreturnする
    int size(int x) {
        return siz[leader(x)];
    }
    int connectedComponents() {
        return connected_components;
    }
    vector<vector<int>> groups() {
        vector<vector<int>> ret(_n);
        vector<int> group_size(_n, 0), leader_rem(_n);
        for (int i = 0; i < _n; i++) {
            leader_rem[i] = leader(i);
            group_size[leader_rem[i]]++;
        }
        for (int i = 0; i < _n; i++) ret[i].reserve(group_size[i]);
        for (int i = 0; i < _n; i++) {
            ret[leader_rem[i]].push_back(i);
        }
        ret.erase(remove_if(ret.begin(), ret.end(), [](const vector<int>& x) { return x.empty(); }), ret.end());
        return ret;
    }
};
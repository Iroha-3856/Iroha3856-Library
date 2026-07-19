//F = true : すでに辺がある場合でも、多重辺として追加したとみなす（削除するときは一本ずつ削除することになる）
//F = false: すでに辺があるなら、追加しない（削除するときには一括で削除することになる）
template<bool F>
struct OfflineDynamicConnectivity {
    using Edge = pair<int, int>;
    int sz;
    int N, Q;
    RollbackUnionFind UF;
    vector<vector<Edge>> seg;
    map<Edge, int> cnt, appear;
    //閉区間
    vector<pair<Edge, pair<int, int>>> pending;
    vector<pair<int, int>> queryrem;

    OfflineDynamicConnectivity(int n) : UF(n) {
        N = n;
        Q = 0;
    }
    bool insert(int u, int v) {
        if (u > v) swap(u, v);
        Edge e{u, v};
        // cout << "insert : " << u << ", " << v << endl;
        if (F) {
            if (cnt[e]++ == 0) {
                appear[e] = Q;
            }
            return true;
        }
        else {
            if (cnt[e] == 0) {
                cnt[e]++;
                appear[e] = Q;
                return true;
            }
            return false;
        }
    }
    bool erase(int u, int v) {
        if (u > v) swap(u, v);
        Edge e{u, v};
        // cout << "erase : " << u << ", " << v << ": " << appear[e] << endl;
        if (appear.find(e) == appear.end() or appear[e] == -1) return false;
        if (--cnt[e] == 0) {
            pending.push_back({e, {appear[e], Q-1}});
            appear[e] = -1;
        }
        return true;
    }
    void query() {
        Q++;
    }
    void build() {
        for (auto[e, c] : cnt) {
            if (c > 0) {
                pending.push_back({e, {appear[e], Q-1}});
            }
        }
        sz = 1;
        while(sz < Q) sz <<= 1;
        //segup segmenttree
        seg.resize(2*sz);
        for (auto[e, segment] : pending) {
            auto[l, r] = segment;
            // cout << "{" << e.first << ", " << e.second << "} : " << "[" << l << " : " << r << "]" << endl;
            r++;
            l += sz; r += sz;
            for (; l < r; l >>= 1, r >>= 1) {
                if (l&1) seg[l++].push_back(e);
                if (r&1) seg[--r].push_back(e);
            }
        }
    }
    template<class REM>
    void dfs(const REM& rem, int k) {
        for (auto[u, v] : seg[k]) {
            UF.merge(u, v);
        }
        if (k < sz) {
            dfs(rem, 2*k);
            dfs(rem, 2*k+1);
        }
        else {
            int x = k-sz;
            if (x < Q) rem(x);
        }
        for (auto[u, v] : seg[k]) {
            UF.undo();
        }
    }
    //引数で何を返すかを渡す
    template<class REM> 
    void run(const REM& rem) {
        build();
        dfs(rem, 1);
    }
};
//部分永続 Union-Find。merge は現在時刻、query は任意の過去時刻
//merge 償却 O(log N)、root O(log N)、size O(log N)。時刻 0 が初期状態
struct PartiallyPersistentUnionFind {
    int N, now = 0;
    vector<int> parent, parentTime;
    vector<vector<pair<int, int>>> sizeHistory;

    PartiallyPersistentUnionFind(int n) : N(n), parent(n, -1),
                                          parentTime(n, numeric_limits<int>::max()),
                                          sizeHistory(n, vector<pair<int, int>>(1, {0, 1})) {}

    int root(int v, int t) const {
        assert(0 <= v and v < N and 0 <= t and t <= now);
        while (parentTime[v] <= t) v = parent[v];
        return v;
    }
    bool same(int u, int v, int t) const { return root(u, t) == root(v, t); }
    int size(int v, int t) const {
        v = root(v, t);
        const auto& H = sizeHistory[v];
        auto it = upper_bound(H.begin(), H.end(), pair<int, int>{t, INF});
        return prev(it)->second;
    }
    bool merge(int u, int v) {
        now++;
        u = root(u, now - 1); v = root(v, now - 1);
        if (u == v) return false;
        int su = size(u, now - 1), sv = size(v, now - 1);
        if (su < sv) swap(u, v), swap(su, sv);
        parent[v] = u;
        parentTime[v] = now;
        sizeHistory[u].push_back({now, su + sv});
        return true;
    }
};

// 部分永続 Union-Find。merge は現在時刻、query は任意の過去時刻
// merge 償却 O(log N)、root O(log N)、size O(log N)。時刻 0 が初期状態
// 使い方:
// 各イベントごとに uf.merge(u, v) を呼び、その直後の時刻を uf.now から得る。
// uf.same(u, v, time), uf.size(v, time) で time 時点の状態を調べる。
// 既に同じ成分への merge でも時刻は進むため、入力イベント番号と時刻を一致させられる。
// 使いどころ: 辺が追加されるだけの時系列で、「時刻 t には連結だったか」を後から調べる場合。
// 具体例: 時刻1で merge(0, 1)、時刻2で merge(1, 2) なら same(0, 2, 1)=false、same(0, 2, 2)=true。
// 過去へ戻って別の merge を分岐させることはできない。その場合は path copying を使う。
struct PartiallyPersistentUnionFind {
    int N, now = 0;
    vector<int> parent, parentTime;
    vector<vector<pair<int, int>>> sizeHistory;

    // N 個の独立頂点を時刻 0 の状態として構築する。
    PartiallyPersistentUnionFind(int n) : N(n), parent(n, -1),
                                          parentTime(n, numeric_limits<int>::max()),
                                          sizeHistory(n, vector<pair<int, int>>(1, {0, 1})) {}

    // 時刻 t における v の根を返す。0 <= t <= now が必要。
    int root(int v, int t) const {
        assert(0 <= v and v < N and 0 <= t and t <= now);
        while (parentTime[v] <= t) v = parent[v];
        return v;
    }
    // 時刻 t に u と v が同じ成分だったかを返す。
    bool same(int u, int v, int t) const { return root(u, t) == root(v, t); }
    // 時刻 t に v が属した成分の頂点数を返す。
    int size(int v, int t) const {
        v = root(v, t);
        const auto& H = sizeHistory[v];
        auto it = upper_bound(H.begin(), H.end(),
                              pair<int, int>{t, numeric_limits<int>::max()});
        return prev(it)->second;
    }
    // 現在状態で u と v を併合し、時刻を 1 進める。併合済みなら false。
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

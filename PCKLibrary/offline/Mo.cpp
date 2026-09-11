//Mo's algorithm。半開区間 [l, r)、add / erase が O(T) なら O((N+Q)sqrt(Q)T)
struct Mo {
    struct Query { int l, r, id; };
    int N;
    vector<Query> query;

    Mo(int n) : N(n) {}
    int addQuery(int l, int r) {
        assert(0 <= l and l <= r and r <= N);
        int id = (int)query.size();
        query.push_back({l, r, id});
        return id;
    }
    template<class Add, class Erase, class Answer>
    void run(const Add& add, const Erase& erase, const Answer& answer) const {
        int Q = (int)query.size();
        int block = max(1, N / max(1, (int)sqrt(max(1, Q))));
        vector<Query> order = query;
        sort(order.begin(), order.end(), [&](Query a, Query b) {
            int x = a.l / block, y = b.l / block;
            if (x != y) return x < y;
            return x & 1 ? a.r > b.r : a.r < b.r;
        });
        int l = 0, r = 0;
        for (Query q : order) {
            while (q.l < l) add(--l);
            while (r < q.r) add(r++);
            while (l < q.l) erase(l++);
            while (q.r < r) erase(--r);
            answer(q.id);
        }
    }
};

//applyUpdate(t, forward, l, r) は t 番目の更新を適用・巻き戻し、区間内なら集計も直す
struct MoWithUpdates {
    struct Query { int l, r, time, id; };
    int N, updates = 0;
    vector<Query> query;

    MoWithUpdates(int n) : N(n) {}
    void addUpdate() { updates++; }
    int addQuery(int l, int r) {
        int id = (int)query.size();
        query.push_back({l, r, updates, id});
        return id;
    }
    template<class Add, class Erase, class ApplyUpdate, class Answer>
    void run(const Add& add, const Erase& erase, const ApplyUpdate& applyUpdate,
             const Answer& answer) const {
        int block = max(1, (int)pow(max(1, N), 2.0 / 3.0));
        vector<Query> order = query;
        sort(order.begin(), order.end(), [&](Query a, Query b) {
            if (a.l / block != b.l / block) return a.l < b.l;
            if (a.r / block != b.r / block) return a.r < b.r;
            return a.time < b.time;
        });
        int l = 0, r = 0, time = 0;
        for (Query q : order) {
            while (time < q.time) applyUpdate(time++, true, l, r);
            while (q.time < time) applyUpdate(--time, false, l, r);
            while (q.l < l) add(--l);
            while (r < q.r) add(r++);
            while (l < q.l) erase(l++);
            while (q.r < r) erase(--r);
            answer(q.id);
        }
    }
};

//木上 Mo。toggle(v) は頂点 v の採用状態を反転する。パスクエリは両端を含む
struct TreeMo {
    struct Query { int l, r, extra, id; };
    int N, log, timer = 0;
    vector<vector<int>> G, parent;
    vector<int> depth, in, out, euler;
    vector<Query> query;

    TreeMo(const vector<vector<int>>& g, int root = 0) : N((int)g.size()), G(g), depth(N),
        in(N), out(N), euler(2 * N) {
        log = 1;
        while ((1 << log) < N) log++;
        parent.assign(log, vector<int>(N, -1));
        dfs(root, -1);
        for (int k = 0; k + 1 < log; k++) for (int v = 0; v < N; v++) {
            if (parent[k][v] != -1) parent[k + 1][v] = parent[k][parent[k][v]];
        }
    }
    void dfs(int v, int p) {
        parent[0][v] = p;
        in[v] = timer; euler[timer++] = v;
        for (int to : G[v]) if (to != p) depth[to] = depth[v] + 1, dfs(to, v);
        out[v] = timer; euler[timer++] = v;
    }
    int lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int d = depth[u] - depth[v];
        for (int k = 0; k < log; k++) if (d >> k & 1) u = parent[k][u];
        if (u == v) return u;
        for (int k = log - 1; k >= 0; k--) if (parent[k][u] != parent[k][v]) {
            u = parent[k][u]; v = parent[k][v];
        }
        return parent[0][u];
    }
    int addQuery(int u, int v) {
        if (in[u] > in[v]) swap(u, v);
        int w = lca(u, v), id = (int)query.size();
        if (w == u) query.push_back({in[u], in[v] + 1, -1, id});
        else query.push_back({out[u], in[v] + 1, w, id});
        return id;
    }
    template<class Toggle, class Answer>
    void run(const Toggle& toggle, const Answer& answer) const {
        int block = max(1, (int)sqrt(max(1, 2 * N)));
        vector<Query> order = query;
        sort(order.begin(), order.end(), [&](Query a, Query b) {
            if (a.l / block != b.l / block) return a.l < b.l;
            return (a.l / block) & 1 ? a.r > b.r : a.r < b.r;
        });
        int l = 0, r = 0;
        for (Query q : order) {
            while (q.l < l) toggle(euler[--l]);
            while (r < q.r) toggle(euler[r++]);
            while (l < q.l) toggle(euler[l++]);
            while (q.r < r) toggle(euler[--r]);
            if (q.extra != -1) toggle(q.extra);
            answer(q.id);
            if (q.extra != -1) toggle(q.extra);
        }
    }
};

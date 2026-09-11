//最大流 Ford-Fulkerson。O(FE)。流量が小さい場合に使う
struct FordFulkerson {
    struct Edge {
        int to, rev;
        ll cap;
    };
    vector<vector<Edge>> G;
    vector<bool> used;

    FordFulkerson(int N) : G(N), used(N) {}
    void addEdge(int from, int to, ll cap) {
        G[from].push_back({to, (int)G[to].size(), cap});
        G[to].push_back({from, (int)G[from].size() - 1, 0});
    }
    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        used[v] = true;
        for (Edge& e : G[v]) {
            if (e.cap == 0 or used[e.to]) continue;
            ll d = dfs(e.to, t, min(f, e.cap));
            if (d == 0) continue;
            e.cap -= d;
            G[e.to][e.rev].cap += d;
            return d;
        }
        return 0;
    }
    ll maxFlow(int s, int t) {
        assert(s != t);
        ll ret = 0, f;
        do {
            fill(used.begin(), used.end(), false);
            f = dfs(s, t, INF);
            ret += f;
        } while (f);
        return ret;
    }
};

//依存: flow/Dinic.cpp
//各辺 low <= flow <= high の実行可能流と s-t 最大流。値は ll に収まること
struct LowerBoundFlow {
    struct BoundedEdge {
        int from, to;
        ll low, high;
    };
    int N;
    vector<BoundedEdge> edges;

    LowerBoundFlow(int n) : N(n) {}
    void addEdge(int from, int to, ll low, ll high) {
        assert(0 <= low and low <= high);
        edges.push_back({from, to, low, high});
    }
    optional<ll> maxFlow(int s, int t) const {
        int ss = N, tt = N + 1;
        Dinic mf(N + 2);
        vector<ll> balance(N);
        for (auto e : edges) {
            mf.addEdge(e.from, e.to, e.high - e.low);
            balance[e.from] -= e.low;
            balance[e.to] += e.low;
        }
        int backIndex = (int)mf.G[t].size();
        mf.addEdge(t, s, INF);
        ll need = 0;
        for (int v = 0; v < N; v++) {
            if (balance[v] > 0) mf.addEdge(ss, v, balance[v]), need += balance[v];
            if (balance[v] < 0) mf.addEdge(v, tt, -balance[v]);
        }
        if (mf.maxFlow(ss, tt) != need) return nullopt;
        ll base = INF - mf.G[t][backIndex].cap;
        int rev = mf.G[t][backIndex].rev;
        mf.G[t][backIndex].cap = 0;
        mf.G[s][rev].cap = 0;
        for (int v = 0; v < N + 2; v++) for (auto& e : mf.G[v]) {
            if (v == ss or v == tt or e.to == ss or e.to == tt) e.cap = 0;
        }
        return base + mf.maxFlow(s, t);
    }
};

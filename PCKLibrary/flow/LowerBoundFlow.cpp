// 依存: flow/Dinic.cpp
// 各辺 low <= flow <= high の実行可能流と s-t 最大流。値は ll に収まること
// 使い方:
// LowerBoundFlow flow(N); flow.addEdge(u, v, low, high); で下限・上限付き有向辺を追加する。
// auto value = flow.maxFlow(s, t); が nullopt なら実行可能流なし、そうでなければ最大 s-t 流量。
// s と t は異なる頂点でなければならない。この API は各辺の実際の流量を復元しない。
// 単なる circulation は別途頂点を s, t として設計する。
// 使いどころ: 各辺に「最低でも low 流す」という義務がある最大流問題。
// 具体例: 0->1 に [2, 5] の辺だけがあるなら maxFlow(0, 1)=5。high<low は許されない。
// 内部では low を先に流したとみなし、各頂点の流入出不足を super source/sink で補う。
struct LowerBoundFlow {
    struct BoundedEdge {
        int from, to;
        ll low, high;
    };
    int N;
    vector<BoundedEdge> edges;

    // N 頂点の下限制約付き flow 問題を作る。
    LowerBoundFlow(int n) : N(n) {}
    // 有向辺 from->to に low 以上 high 以下の流量制約を追加する。
    void addEdge(int from, int to, ll low, ll high) {
        assert(0 <= from and from < N and 0 <= to and to < N);
        assert(0 <= low and low <= high);
        edges.push_back({from, to, low, high});
    }
    // 全下限を満たす s-t 最大流を返す。実行可能流が存在しなければ nullopt。
    optional<ll> maxFlow(int s, int t) const {
        assert(0 <= s and s < N and 0 <= t and t < N and s != t);
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

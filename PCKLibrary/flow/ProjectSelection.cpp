//依存: flow/Dinic.cpp
//二値選択の利得最大化。selected[v] = true を s 側とする
struct ProjectSelection {
    int N, source, sink;
    ll constant = 0;
    Dinic mf;

    ProjectSelection(int n) : N(n), source(n), sink(n + 1), mf(n + 2) {}
    //v を選ぶと profit。負なら選択への罰金
    void addProfit(int v, ll profit) {
        if (profit >= 0) constant += profit, mf.addEdge(source, v, profit);
        else mf.addEdge(v, sink, -profit);
    }
    //u を選び v を選ばない場合に penalty
    void addPenalty(int u, int v, ll penalty) {
        assert(penalty >= 0);
        mf.addEdge(u, v, penalty);
    }
    void addImplication(int u, int v) { mf.addEdge(u, v, INF); }
    pair<ll, vector<bool>> solve() {
        ll value = constant - mf.maxFlow(source, sink);
        vector<bool> side = mf.minCut(source), selected(N);
        for (int v = 0; v < N; v++) selected[v] = side[v];
        return {value, selected};
    }
};

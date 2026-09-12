// 依存: flow/Dinic.cpp
// 二値選択の利得最大化。selected[v] = true を s 側とする
// 使い方:
// ProjectSelection problem(N); problem.addProfit(v, gain); で単独選択の利得を加える。
// addPenalty(u, v, p) は u を選び v を選ばない場合だけ p を失う条件、addImplication はその禁止版。
// auto [best, selected] = problem.solve(); が最大利得と各項目の選択結果を返す。
// penalty は非負。二変数の任意の利得表ではなく、s-t cut に落とせる劣モジュラな条件に使う。
// solve は内部 flow をコピーして計算するため破壊的でなく、同じ問題に複数回呼んでもよい。
// 使いどころ: プロジェクト選択、maximum closure、依存関係つき利益最大化。
// 具体例: 項目0の利益5、項目1の利益4、0=>1 なら両方選んだ利益9が最適になる。
// selected=true は source 側。INF は有限な利得・罰金の総和より十分大きく取る。
struct ProjectSelection {
    int N, source, sink;
    ll constant = 0;
    Dinic mf;

    // N 個の bool 選択変数を持つ空の最大化問題を作る。
    ProjectSelection(int n) : N(n), source(n), sink(n + 1), mf(n + 2) {}
    // v を選んだときの利得 profit を加える。負値は選択への罰金を表す。
    void addProfit(int v, ll profit) {
        if (profit >= 0) constant += profit, mf.addEdge(source, v, profit);
        else mf.addEdge(v, sink, -profit);
    }
    // u を選び v を選ばない場合に限って非負 penalty を課す。
    void addPenalty(int u, int v, ll penalty) {
        assert(penalty >= 0);
        mf.addEdge(u, v, penalty);
    }
    // u を選ぶなら必ず v も選ぶという含意を追加する。
    void addImplication(int u, int v) { mf.addEdge(u, v, INF); }
    // 最大利得と、それを達成する selected 配列を最小カットから復元する。元の flow は変更しない。
    pair<ll, vector<bool>> solve() const {
        Dinic flow = mf;
        ll value = constant - flow.maxFlow(source, sink);
        vector<bool> side = flow.minCut(source), selected(N);
        for (int v = 0; v < N; v++) selected[v] = side[v];
        return {value, selected};
    }
};

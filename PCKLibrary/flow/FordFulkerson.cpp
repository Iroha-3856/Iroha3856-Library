// 最大流 Ford-Fulkerson。O(FE)。流量が小さい場合に使う
// 使い方:
// FordFulkerson flow(N); flow.addEdge(from, to, capacity); の後 flow.maxFlow(s, t) を呼ぶ。
// 整数容量で、最大流量 F が小さい場合の短い実装。大容量・密なグラフでは Dinic を使う。
// maxFlow は残余容量を破壊的に更新するため、同じネットワークを再利用すると追加流になる。
// 使いどころ: 容量がすべて小さい整数で、Dinic より短いコードを優先できる場合。
// 具体例: 最大流量 F<=100 程度なら、最大でも F 回の増加路探索で終わる。
// 実数容量では DFS の選び方により停止性が保証されないため、この ll 実装の整数容量を使う。
struct FordFulkerson {
    struct Edge {
        int to, rev;
        ll cap;
    };
    vector<vector<Edge>> G;
    vector<bool> used;

    // N 頂点の空の残余グラフを作る。
    FordFulkerson(int N) : G(N), used(N) {}
    // 容量 cap の有向辺 from->to と容量 0 の逆辺を追加する。
    void addEdge(int from, int to, ll cap) {
        G[from].push_back({to, (int)G[to].size(), cap});
        G[to].push_back({from, (int)G[from].size() - 1, 0});
    }
    // v から t への残余路を DFS し、最大 f の流量を一度だけ流す。
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
    // 増加路がなくなるまで DFS を繰り返し、s-t 最大流を返す。
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

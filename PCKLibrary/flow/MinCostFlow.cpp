// 最小費用流。負辺対応、負閉路なしを仮定
// 初回 Bellman-Ford O(VE)、以後は各増加路ごとに O(E log V)
// 使い方:
// MinCostFlow flow(N); flow.addEdge(from, to, capacity, cost); で有向辺を追加する。
// auto [sent, totalCost] = flow.flow(s, t, limit); は limit 以下を可能なだけ流す。
// sent<limit なら指定量を流せない。呼出しは残余グラフを更新するので、再度呼ぶと追加分を流す。
// 負辺は扱えるが、source から到達可能な負閉路がないことと積が ll に収まることを仮定する。
// 使いどころ: 指定量を運ぶ費用の最小化、重み付き matching、需要供給問題。
// 具体例: 容量2・単価3の一本道なら flow(s, t, 1)={1, 3}、続く flow(s, t, 2)={1, 3}。
// 返る sent は今回の呼出しで追加した流量であり、過去の累積流量ではない。
struct MinCostFlow {
    struct Edge {
        int to, rev;
        ll cap, cost;
    };
    int N;
    vector<vector<Edge>> G;

    // n 頂点の空の残余グラフを作る。
    MinCostFlow(int n) : N(n), G(n) {}
    // 容量 cap、単位費用 cost の有向辺と対応する逆辺を追加する。
    void addEdge(int from, int to, ll cap, ll cost) {
        G[from].push_back({to, (int)G[to].size(), cap, cost});
        G[to].push_back({from, (int)G[from].size() - 1, 0, -cost});
    }
    // s から t へ limit 以下を最小費用で流し、{実流量, 総費用} を返す。
    pair<ll, ll> flow(int s, int t, ll limit = INF) {
        vector<ll> potential(N, INF);
        potential[s] = 0;
        for (int iter = 0; iter < N; iter++) {
            bool update = false;
            for (int v = 0; v < N; v++) if (potential[v] != INF) {
                for (const Edge& e : G[v]) if (e.cap and chmin(potential[e.to], potential[v] + e.cost)) {
                    update = true;
                }
            }
            if (!update) break;
        }
        for (ll& x : potential) if (x == INF) x = 0;
        ll sent = 0, cost = 0;
        vector<ll> dist(N);
        vector<int> prevV(N), prevE(N);
        while (sent < limit) {
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> Q;
            Q.push({0, s});
            while (!Q.empty()) {
                auto [d, v] = Q.top(); Q.pop();
                if (dist[v] != d) continue;
                for (int i = 0; i < (int)G[v].size(); i++) {
                    Edge& e = G[v][i];
                    if (!e.cap) continue;
                    ll nd = d + e.cost + potential[v] - potential[e.to];
                    if (chmin(dist[e.to], nd)) {
                        prevV[e.to] = v; prevE[e.to] = i;
                        Q.push({nd, e.to});
                    }
                }
            }
            if (dist[t] == INF) break;
            for (int v = 0; v < N; v++) if (dist[v] != INF) potential[v] += dist[v];
            ll add = limit - sent;
            for (int v = t; v != s; v = prevV[v]) add = min(add, G[prevV[v]][prevE[v]].cap);
            sent += add;
            cost += add * (potential[t] - potential[s]);
            for (int v = t; v != s; v = prevV[v]) {
                Edge& e = G[prevV[v]][prevE[v]];
                e.cap -= add;
                G[v][e.rev].cap += add;
            }
        }
        return {sent, cost};
    }
};

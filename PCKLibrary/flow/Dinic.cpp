// 最大流 Dinic。O(V^2 E)
// 使い方:
// Dinic flow(N); flow.addEdge(from, to, capacity); で有向辺を追加する。
// ll value = flow.maxFlow(source, sink); を呼ぶと残余容量を破壊的に更新する。
// 直後の flow.minCut(source)[v] は残余グラフで source 側に属する頂点かを表す。
// 無向容量辺は必要な意味に応じて両方向へ addEdge する。
// 使いどころ: 容量制約下の最大輸送量、二部 matching、最小 cut への帰着。
// 具体例: 0->1 容量3、1->2 容量2 だけなら maxFlow(0, 2)=2。
// addEdge は有向辺一つを作る。互いに独立な両方向容量 c が必要なら二回 addEdge する。
struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };
    int N;
    vector<vector<Edge>> G;
    vector<int> level, iter;

    // n 頂点の空の残余グラフを作る。
    Dinic(int n) : N(n), G(n), level(n), iter(n) {}
    // 容量 cap の有向辺 from->to と容量 0 の逆辺を追加する。
    void addEdge(int from, int to, ll cap) {
        G[from].push_back({to, (int)G[to].size(), cap});
        G[to].push_back({from, (int)G[from].size() - 1, 0});
    }
    // 残余容量が正の辺だけで source s からの level graph を構築する。
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> Q;
        level[s] = 0; Q.push(s);
        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            for (auto e : G[v]) {
                if (e.cap and level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    Q.push(e.to);
                }
            }
        }
    }
    // level が増加する辺だけを使い、v から t へ最大 f の flow を一度流す。
    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            Edge& e = G[v][i];
            if (e.cap == 0 or level[v] >= level[e.to]) continue;
            ll d = dfs(e.to, t, min(f, e.cap));
            if (d == 0) continue;
            e.cap -= d;
            G[e.to][e.rev].cap += d;
            return d;
        }
        return 0;
    }
    // s から t への最大流を返し、G の残余容量を破壊的に更新する。
    ll maxFlow(int s, int t) {
        assert(s != t);
        ll ret = 0, f;
        while (true) {
            bfs(s);
            if (level[t] == -1) return ret;
            fill(iter.begin(), iter.end(), 0);
            while ((f = dfs(s, t, INF))) ret += f;
        }
    }
    // maxFlow 後、残余グラフで s から到達可能な最小カットの source 側を返す。
    vector<bool> minCut(int s) const {
        vector<bool> used(N);
        queue<int> Q;
        used[s] = true; Q.push(s);
        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            for (const Edge& e : G[v]) if (e.cap and !used[e.to]) {
                used[e.to] = true;
                Q.push(e.to);
            }
        }
        return used;
    }
};

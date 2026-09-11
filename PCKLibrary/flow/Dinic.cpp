//最大流 Dinic。O(V^2 E)
struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };
    int N;
    vector<vector<Edge>> G;
    vector<int> level, iter;

    Dinic(int n) : N(n), G(n), level(n), iter(n) {}
    void addEdge(int from, int to, ll cap) {
        G[from].push_back({to, (int)G[to].size(), cap});
        G[to].push_back({from, (int)G[from].size() - 1, 0});
    }
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
    //maxFlow(s, t) 後、残余グラフで s から到達可能な最小カット側
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

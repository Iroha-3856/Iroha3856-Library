//根付き・根なし木の乱択 hash。O(N)。一致は同型の必要条件で、衝突確率を持つ
//同じ seed を持つ TreeHash 同士で比較する
struct TreeHash {
    using ull = unsigned long long;
    ull seed;

    TreeHash(ull s = chrono::steady_clock::now().time_since_epoch().count()) : seed(s) {}
    ull splitmix64(ull x) const {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    pair<ull, int> dfs(const vector<vector<int>>& G, int v, int p) const {
        vector<pair<ull, int>> child;
        for (int to : G[v]) if (to != p) child.push_back(dfs(G, to, v));
        int height = 0;
        ull h = splitmix64(seed + 1);
        for (auto [x, d] : child) {
            height = max(height, d + 1);
            h += splitmix64(x + splitmix64(seed + d));
        }
        return {splitmix64(h + child.size()), height};
    }
    ull rooted(const vector<vector<int>>& G, int root) const { return dfs(G, root, -1).first; }
    vector<int> centers(const vector<vector<int>>& G) const {
        int N = (int)G.size();
        if (N <= 2) {
            vector<int> ret(N);
            iota(ret.begin(), ret.end(), 0);
            return ret;
        }
        vector<int> degree(N);
        queue<int> que;
        for (int v = 0; v < N; v++) {
            degree[v] = (int)G[v].size();
            if (degree[v] <= 1) que.push(v);
        }
        int remain = N;
        while (remain > 2) {
            int cnt = (int)que.size();
            remain -= cnt;
            while (cnt--) {
                int v = que.front(); que.pop();
                for (int to : G[v]) if (--degree[to] == 1) que.push(to);
            }
        }
        vector<int> ret;
        while (!que.empty()) ret.push_back(que.front()), que.pop();
        return ret;
    }
    pair<ull, ull> unrooted(const vector<vector<int>>& G) const {
        vector<int> C = centers(G);
        ull a = C.empty() ? 0 : rooted(G, C[0]);
        ull b = C.size() == 2 ? rooted(G, C[1]) : a;
        if (a > b) swap(a, b);
        return {a, b};
    }
};

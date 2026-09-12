// 根付き・根なし木の乱択 hash。O(N)。一致は同型の必要条件で、衝突確率を持つ
// 同じ seed を持つ TreeHash 同士で比較する
// 使い方:
// TreeHash hash(seed); を一つ作り、比較する全木へ同じインスタンスを使う。
// hash.rooted(G, root) は根付き木、hash.unrooted(G) は根なし木の正規化した二つの hash を返す。
// hash.centers(G) で中心も取得できる。hash が一致しても低確率の衝突がある点に注意する。
// 使いどころ: 子の並び順を持たない木同士が同型かを、多数または高速に比較したい場合。
// 具体例: 頂点番号だけを付け替えた4頂点パス同士の unrooted hash は等しく、4頂点 star とは異なる。
// 根なし木は中心が二つの場合があるため、両中心で根付けた hash の昇順 pair を比較する。
// 異なる hash なら必ず非同型。一致時だけ衝突可能性があるので、厳密判定が必要なら正規形文字列を使う。
struct TreeHash {
    using ull = unsigned long long;
    ull seed;

    // 木間で共有する seed を設定する。省略時は実行ごとに変わる seed を使う。
    TreeHash(ull s = chrono::steady_clock::now().time_since_epoch().count()) : seed(s) {}
    // 64 bit 整数 x を十分拡散した疑似乱数へ写す。
    ull splitmix64(ull x) const {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    // v を根とする部分木の {hash, 高さ} を子の順序に依存せず計算する。
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
    // 指定した root を持つ根付き木の hash を返す。
    ull rooted(const vector<vector<int>>& G, int root) const { return dfs(G, root, -1).first; }
    // 葉を同時に除く方法で根なし木の中心を一つまたは二つ返す。
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
    // 各中心で根付けた hash を昇順の組として返し、根なし木を比較可能にする。
    pair<ull, ull> unrooted(const vector<vector<int>>& G) const {
        vector<int> C = centers(G);
        ull a = C.empty() ? 0 : rooted(G, C[0]);
        ull b = C.size() == 2 ? rooted(G, C[1]) : a;
        if (a > b) swap(a, b);
        return {a, b};
    }
};

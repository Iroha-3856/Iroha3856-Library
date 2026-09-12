// DSU on Tree。add / remove が O(T) なら全体 O(NT log N)
// answer(v) は v の部分木がすべて add された時に呼ばれる
// 使い方:
// DSUOnTree sack(G, root); を作り、sack.run(add, remove, answer) を一度呼ぶ。
// add(v) / remove(v) は頂点 v の寄与を状態へ追加・削除し、互いに逆操作とする。
// answer(v) の呼出時には v の部分木だけが状態にあるので、頂点 v の答えを保存する。
// light child の状態は消し、最大の heavy child の状態だけを再利用する。
// 例:
// vector<int> countColor(C), answer(N);
// sack.run([&](int v){ countColor[color[v]]++; },
// [&](int v){ countColor[color[v]]--; },
// [&](int v){ answer[v] = countColor[target[v]]; });
// run 終了後は根の部分木、つまり全頂点が状態に残る。G は連結な無向木とする。
// 使いどころ: 各部分木の色種類数・頻度・最頻値など、頂点を一つずつ足し引きできる集計。
// 「DSU」という名前だが Union-Find は使わない。大きい子の集計を残す small-to-large の考え方を指す。
// answer 内で状態を変更せず、add と remove が厳密に逆になるように書くと安全。
struct DSUOnTree {
    int N, root, timer = 0;
    vector<vector<int>> G;
    vector<int> parent, sub, heavy, in, out, euler;

    // g を treeRoot で根付け、部分木区間と heavy child を前計算する。
    DSUOnTree(const vector<vector<int>>& g, int treeRoot = 0) : N((int)g.size()), root(treeRoot),
        G(g), parent(N, -1), sub(N), heavy(N, -1), in(N), out(N), euler(N) {
        assert(N > 0);
        dfsBuild(root, -1);
    }
    // v の部分木サイズ・heavy child・Euler Tour の半開区間を求める。
    void dfsBuild(int v, int p) {
        parent[v] = p;
        in[v] = timer;
        euler[timer++] = v;
        sub[v] = 1;
        for (int to : G[v]) if (to != p) {
            dfsBuild(to, v);
            sub[v] += sub[to];
            if (heavy[v] == -1 or sub[heavy[v]] < sub[to]) heavy[v] = to;
        }
        out[v] = timer;
    }
    // v の部分木を処理し、keep=false なら処理後にその寄与をすべて削除する。
    template<class Add, class Remove, class Answer>
    void dfs(int v, bool keep, const Add& add, const Remove& remove, const Answer& answer) {
        for (int to : G[v]) if (to != parent[v] and to != heavy[v]) {
            dfs(to, false, add, remove, answer);
        }
        if (heavy[v] != -1) dfs(heavy[v], true, add, remove, answer);
        for (int to : G[v]) if (to != parent[v] and to != heavy[v]) {
            for (int i = in[to]; i < out[to]; i++) add(euler[i]);
        }
        add(v);
        answer(v);
        if (!keep) for (int i = in[v]; i < out[v]; i++) remove(euler[i]);
    }
    // 根から DSU on Tree を開始し、各頂点について answer を一度ずつ呼ぶ。
    template<class Add, class Remove, class Answer>
    void run(const Add& add, const Remove& remove, const Answer& answer) {
        dfs(root, true, add, remove, answer);
    }
};

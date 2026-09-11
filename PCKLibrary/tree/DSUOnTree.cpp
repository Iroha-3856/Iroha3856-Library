//DSU on Tree。add / remove が O(T) なら全体 O(NT log N)
//answer(v) は v の部分木がすべて add された時に呼ばれる
struct DSUOnTree {
    int N, timer = 0;
    vector<vector<int>> G;
    vector<int> parent, sub, heavy, in, out, euler;

    DSUOnTree(const vector<vector<int>>& g, int root = 0) : N((int)g.size()), G(g),
        parent(N, -1), sub(N), heavy(N, -1), in(N), out(N), euler(N) {
        dfsBuild(root, -1);
    }
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
    template<class Add, class Remove, class Answer>
    void run(const Add& add, const Remove& remove, const Answer& answer, int root = 0) {
        dfs(root, true, add, remove, answer);
    }
};

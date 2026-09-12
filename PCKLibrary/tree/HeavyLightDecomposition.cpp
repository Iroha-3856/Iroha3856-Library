// Heavy-Light Decomposition。構築 O(N)、パス分解 O(log N)
// 頂点・部分木は in の半開区間。path の reversed は r-1 から l へ読むことを表す
// 使い方:
// HeavyLightDecomposition hld(G, root); とし、配列上の hld.in[v] に頂点値を置く。
// hld.vertex[i] は Euler Tour 上の添字 i に対応する元頂点で、in の逆写像になる。
// hld.subtree(v) は頂点部分木、subtree(v, true) は親辺を除く辺部分木の [l, r)。
// hld.path(u, v, edge) の各 Segment を返却順に処理する。edge=true なら LCA の頂点を除く。
// reversed=false は l から r-1、true は r-1 から l へ読む。
// 可換な区間積なら reversed は無視でき、非可換なら正順・逆順の区間積を用意する。
// 例（可換な頂点和）:
// ll answer = 0;
// for (auto [l, r, reversed] : hld.path(u, v)) answer += seg.prod(l, r);
// G は空でない連結な無向木とする。lca(u, v) も O(log N) で利用できる。
// 使いどころ: 木上のパス・部分木 query を Segment Tree の区間 query へ変換する場合。
// 具体例: path(v, v) は頂点版なら [in[v], in[v]+1) 一区間、edge=true なら空列になる。
// heavy edge は最大部分木の子へ向かう辺で、任意の根から葉へのパスが通る light edge は O(log N) 本。
// 辺値は「深い側の頂点」の in に置く。この規約なら edge=true で LCA の格納位置だけを除けばよい。
struct HeavyLightDecomposition {
    struct Segment {
        int l, r;
        bool reversed;
    };
    int N;
    vector<vector<int>> G;
    vector<int> parent, depth, sub, heavy, head, in, out, vertex;
    int timer = 0;

    // g を root で根付け、heavy path と Euler Tour 添字を構築する。
    HeavyLightDecomposition(const vector<vector<int>>& g, int root = 0) : N((int)g.size()), G(g),
        parent(N, -1), depth(N), sub(N), heavy(N, -1), head(N), in(N), out(N), vertex(N) {
        assert(N > 0);
        dfsSize(root, -1);
        dfsOrder(root, root);
    }
    // 親・深さ・部分木サイズを求め、最大部分木の子を heavy[v] に選ぶ。
    void dfsSize(int v, int p) {
        parent[v] = p;
        sub[v] = 1;
        int best = 0;
        for (int to : G[v]) if (to != p) {
            depth[to] = depth[v] + 1;
            dfsSize(to, v);
            sub[v] += sub[to];
            if (best < sub[to]) best = sub[to], heavy[v] = to;
        }
    }
    // heavy child を最初に辿って、各部分木を連続区間へ配置する。
    void dfsOrder(int v, int h) {
        head[v] = h;
        in[v] = timer;
        vertex[timer++] = v;
        if (heavy[v] != -1) dfsOrder(heavy[v], h);
        for (int to : G[v]) if (to != parent[v] and to != heavy[v]) dfsOrder(to, to);
        out[v] = timer;
    }
    // 頂点 u, v の最近共通祖先を O(log N) で返す。
    int lca(int u, int v) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) u = parent[head[u]];
            else v = parent[head[v]];
        }
        return depth[u] < depth[v] ? u : v;
    }
    // v の部分木に対応する半開区間を返す。edge=true なら v 自身を除く。
    pair<int, int> subtree(int v, bool edge = false) const {
        return {in[v] + edge, out[v]};
    }
    // u から v へのパスを、走査方向付きの O(log N) 個の半開区間へ分解する。
    vector<Segment> path(int u, int v, bool edge = false) const {
        vector<Segment> left, right;
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                left.push_back({in[head[u]], in[u] + 1, true});
                u = parent[head[u]];
            } else {
                right.push_back({in[head[v]], in[v] + 1, false});
                v = parent[head[v]];
            }
        }
        if (depth[u] > depth[v]) left.push_back({in[v] + edge, in[u] + 1, true});
        else right.push_back({in[u] + edge, in[v] + 1, false});
        reverse(right.begin(), right.end());
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }
};

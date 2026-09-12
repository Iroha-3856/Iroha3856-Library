// 木の重心分解。O(N log N)
// parent は重心分解木の親、level は深さ、root は分解木の根
// 使い方:
// 無向木 G から CentroidDecomposition cd(G); と構築する。
// cd.root が重心分解木の根、cd.parent[v] と cd.level[v] が分解木上の親と深さ。
// 元の木の辺は cd.G に残り、parent は元木の親ではない点に注意する。
// 距離クエリでは、各頂点から parent を遡る各重心について距離別情報を別途保持する。
// 例:
// for (int c = v; c != -1; c = cd.parent[c]) 重心 c に対応する情報を更新する。
// G は自己辺・多重辺のない連結な無向木とする。空なら root は -1。
// 使いどころ: 頂点更新と距離条件付き query を、重心の祖先ごとの情報へ分解する場合。
// 重心とは、取り除いた後の各連結成分が元の半分以下になる頂点。そのため分解木の高さは O(log N)。
// parent を一段上がるたびに「より大きい元木の領域を担当する重心」へ移る。
struct CentroidDecomposition {
    int N, root = -1;
    vector<vector<int>> G;
    vector<int> sub, parent, level;
    vector<bool> removed;

    // 無向木 g を再帰的に重心分解し、分解木を構築する。
    CentroidDecomposition(const vector<vector<int>>& g) : N((int)g.size()), G(g), sub(N),
        parent(N, -1), level(N), removed(N) {
        if (N) root = build(0, -1, 0);
    }
    // removed でない連結成分内の部分木サイズを求め、そのサイズを返す。
    int dfsSize(int v, int p) {
        sub[v] = 1;
        for (int to : G[v]) if (to != p and !removed[to]) sub[v] += dfsSize(to, v);
        return sub[v];
    }
    // 大きさ total の成分を v から辿り、重心となる頂点を返す。
    int findCentroid(int v, int p, int total) {
        for (int to : G[v]) if (to != p and !removed[to] and sub[to] * 2 > total) {
            return findCentroid(to, v, total);
        }
        return v;
    }
    // start の成分を分解し、重心を親 p・深さ d として登録して返す。
    int build(int start, int p, int d) {
        int c = findCentroid(start, -1, dfsSize(start, -1));
        parent[c] = p;
        level[c] = d;
        removed[c] = true;
        for (int to : G[c]) if (!removed[to]) build(to, c, d + 1);
        return c;
    }
};

// Static Top Tree。構築 O(N)、この単純な平衡化では頂点値の一点更新 O(log^2 N)
// DP は path / point cluster の両方を表せる型にする。結合の引数順は根から葉の順
// makeVertex(v): 頂点 v の path cluster
// addEdge(path): light child の path を親側境界だけの point cluster にする
// rake(a, b): point cluster の併合
// addVertex(point, vertex): light child 群を頂点 cluster へ付加
// compress(a, b): heavy edge で上下の path cluster を接続
// 使い方:
// 頂点値を外部配列に置き、上記 5 種類の cluster 変換・結合関数を用意して構築する。
// StaticTopTree<DP> tree(G, makeVertex, addEdge, rake, addVertex, compress, root);
// 頂点 v の外部値を変更した後 tree.update(v) を呼ぶと、tree.allProd() が木全体の新しい DP。
// rake は左右交換可能な point cluster の併合、compress は根側から葉側の順を保つ必要がある。
// 単純な頂点和なら addEdge は恒等、残り三つの二項演算は加算にすればよい。
// 使いどころ: 木の形は固定で頂点値だけ変化し、木全体の DP を各更新 O(log^2 N) で再計算したい場合。
// heavy path と light child 群を個数で平衡化するため、cluster 木の高さは最悪 O(log^2 N)。
// path cluster は境界を二つ、point cluster は親側境界だけを持つと考えると五演算の型を決めやすい。
// Rake は同じ親側境界を共有する light subtree を束ね、Compress は heavy path を上から下へつなぐ。
// 具体例: 頂点和では makeVertex(v)=weight[v] とし、外部 weight[v] 更新後に update(v) を呼ぶ。
template<class DP>
struct StaticTopTree {
    enum Type { Vertex, AddEdge, Rake, AddVertex, Compress };
    struct Node {
        Type type;
        int left = -1, right = -1, vertex = -1, parent = -1;
    };
    int N;
    vector<vector<int>> G;
    vector<int> parent, sub, heavy, leaf;
    vector<Node> node;
    vector<DP> value;
    int root;
    function<DP(int)> makeVertex;
    function<DP(const DP&)> addEdge;
    function<DP(const DP&, const DP&)> rake;
    function<DP(const DP&, const DP&)> addVertex;
    function<DP(const DP&, const DP&)> compress;

    // 木 g と五つの cluster 演算から Static Top Tree を構築する。
    StaticTopTree(const vector<vector<int>>& g,
                  function<DP(int)> vertexFunction,
                  function<DP(const DP&)> edgeFunction,
                  function<DP(const DP&, const DP&)> rakeFunction,
                  function<DP(const DP&, const DP&)> vertexMergeFunction,
                  function<DP(const DP&, const DP&)> compressFunction,
                  int treeRoot = 0)
        : N((int)g.size()), G(g), parent(N, -1), sub(N), heavy(N, -1), leaf(N, -1),
          makeVertex(vertexFunction), addEdge(edgeFunction), rake(rakeFunction),
          addVertex(vertexMergeFunction), compress(compressFunction) {
        assert(N > 0);
        dfsSize(treeRoot, -1);
        root = buildPath(treeRoot);
        value.resize(node.size());
        for (int k = 0; k < (int)node.size(); k++) updateNode(k);
    }
    // 元木を根付け、部分木サイズと heavy child を求める。
    void dfsSize(int v, int p) {
        parent[v] = p;
        sub[v] = 1;
        for (int to : G[v]) if (to != p) {
            dfsSize(to, v);
            sub[v] += sub[to];
            if (heavy[v] == -1 or sub[heavy[v]] < sub[to]) heavy[v] = to;
        }
    }
    // 演算種別と子を持つ cluster 節点を追加し、その添字を返す。
    int makeNode(Type type, int l = -1, int r = -1, int v = -1) {
        int k = (int)node.size();
        node.push_back({type, l, r, v, -1});
        if (l != -1) node[l].parent = k;
        if (r != -1) node[r].parent = k;
        return k;
    }
    // cluster 添字列 A[l, r) を type 演算の平衡二分木として併合する。
    int mergeBalanced(const vector<int>& A, int l, int r, Type type) {
        if (l + 1 == r) return A[l];
        int m = (l + r) / 2;
        return makeNode(type, mergeBalanced(A, l, m, type), mergeBalanced(A, m, r, type));
    }
    // 元頂点 v と全 light child を一つの path cluster へ変換する。
    int buildVertex(int v) {
        int base = makeNode(Vertex, -1, -1, v);
        leaf[v] = base;
        vector<int> light;
        for (int to : G[v]) if (to != parent[v] and to != heavy[v]) {
            light.push_back(makeNode(AddEdge, buildPath(to)));
        }
        if (light.empty()) return base;
        int point = mergeBalanced(light, 0, (int)light.size(), Rake);
        return makeNode(AddVertex, point, base);
    }
    // start から heavy path を辿り、各頂点 cluster を Compress で併合する。
    int buildPath(int start) {
        vector<int> path;
        for (int v = start; v != -1; v = heavy[v]) path.push_back(buildVertex(v));
        return mergeBalanced(path, 0, (int)path.size(), Compress);
    }
    // 節点 k の種別に対応する演算を子の value へ適用して value[k] を更新する。
    void updateNode(int k) {
        Node& a = node[k];
        if (a.type == Vertex) value[k] = makeVertex(a.vertex);
        else if (a.type == AddEdge) value[k] = addEdge(value[a.left]);
        else if (a.type == Rake) value[k] = rake(value[a.left], value[a.right]);
        else if (a.type == AddVertex) value[k] = addVertex(value[a.left], value[a.right]);
        else value[k] = compress(value[a.left], value[a.right]);
    }
    // 元頂点 v の値変更を、その leaf から cluster 木の根まで再計算する。
    void update(int v) {
        assert(0 <= v and v < N);
        for (int k = leaf[v]; k != -1; k = node[k].parent) updateNode(k);
    }
    // 現在の木全体を表す根 cluster の DP を返す。
    const DP& allProd() const { return value[root]; }
};

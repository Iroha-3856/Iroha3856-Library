//Static Top Tree。構築 O(N log N)、頂点値の一点更新 O(log N)
//DP は path / point cluster の両方を表せる型にする。結合の引数順は根から葉の順
//makeVertex(v): 頂点 v の path cluster
//addEdge(path): light child の path を親側境界だけの point cluster にする
//rake(a, b): point cluster の併合
//addVertex(point, vertex): light child 群を頂点 cluster へ付加
//compress(a, b): heavy edge で上下の path cluster を接続
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
    void dfsSize(int v, int p) {
        parent[v] = p;
        sub[v] = 1;
        for (int to : G[v]) if (to != p) {
            dfsSize(to, v);
            sub[v] += sub[to];
            if (heavy[v] == -1 or sub[heavy[v]] < sub[to]) heavy[v] = to;
        }
    }
    int makeNode(Type type, int l = -1, int r = -1, int v = -1) {
        int k = (int)node.size();
        node.push_back({type, l, r, v, -1});
        if (l != -1) node[l].parent = k;
        if (r != -1) node[r].parent = k;
        return k;
    }
    int mergeBalanced(const vector<int>& A, int l, int r, Type type) {
        if (l + 1 == r) return A[l];
        int m = (l + r) / 2;
        return makeNode(type, mergeBalanced(A, l, m, type), mergeBalanced(A, m, r, type));
    }
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
    int buildPath(int start) {
        vector<int> path;
        for (int v = start; v != -1; v = heavy[v]) path.push_back(buildVertex(v));
        return mergeBalanced(path, 0, (int)path.size(), Compress);
    }
    void updateNode(int k) {
        Node& a = node[k];
        if (a.type == Vertex) value[k] = makeVertex(a.vertex);
        else if (a.type == AddEdge) value[k] = addEdge(value[a.left]);
        else if (a.type == Rake) value[k] = rake(value[a.left], value[a.right]);
        else if (a.type == AddVertex) value[k] = addVertex(value[a.left], value[a.right]);
        else value[k] = compress(value[a.left], value[a.right]);
    }
    void update(int v) {
        assert(0 <= v and v < N);
        for (int k = leaf[v]; k != -1; k = node[k].parent) updateNode(k);
    }
    const DP& allProd() const { return value[root]; }
};

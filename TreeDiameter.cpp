//木の直径ライブラリ
//verify: https://judge.yosupo.jp/submission/253704
struct TreeDiameter {
    struct Edge {
        int to; long long cost;
    };

    int N;
    vector<vector<Edge>> G;
    int fv0;
    int fv1;
    long long diameter;
    vector<long long> dist0;
    vector<long long> distfv0;
    vector<int> parent;
    bool build_completed;

    TreeDiameter() : TreeDiameter(0) {}
    TreeDiameter(int n) {
        N = n;
        G.resize(N);
        parent.resize(N);
        build_completed = false;
    }
    void addEdge(int u, int v, long long cost = 1) {
        assert(0 <= u and 0 <= v and u < N and v < N);
        G[u].push_back(Edge{v, cost});
        G[v].push_back(Edge{u, cost});
    }
    void calcDistance(int v, int par, vector<long long>& dist) {
        parent[v] = par;
        for (Edge e : G[v]) {
            if (e.to == par) continue;
            dist[e.to] = dist[v] + e.cost;
            calcDistance(e.to, v, dist);
        }
    }
    //必要な情報を構築する
    //O(N)
    void build() {
        build_completed = true;
        dist0.assign(N, 0);
        calcDistance(0, -1, dist0);
        diameter = 0;
        for (int i = 0; i < N; i++) {
            if (diameter < dist0[i]) {
                diameter = dist0[i];
                fv0 = i;
            }
        }
        distfv0.assign(N, 0);
        calcDistance(fv0, -1, distfv0);
        diameter = 0;
        for (int i = 0; i < N; i++) {
            if (diameter < distfv0[i]) {
                diameter = distfv0[i];
                fv1 = i;
            }
        }
    }
    //直径の長さを求める
    //O(1)
    long long diameterCost() {
        assert(build_completed);
        return diameter;
    }
    //直径に含まれる頂点を求める
    //O(N)
    vector<int> diameterPath() {
        assert(build_completed);
        vector<int> ret;
        int now = fv1;
        while(parent[now] != -1) {
            ret.emplace_back(now);
            now = parent[now];
        }
        ret.emplace_back(fv0);
        return ret;
    }
};
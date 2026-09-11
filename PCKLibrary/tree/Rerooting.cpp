//全方位木 DP
//putEdge(x, cost) は隣の頂点で完成した DP を辺越しに変換する
//putVertex(x, v) は子からの値をすべて merge した後、頂点 v を加える
//O(N)、添字は 0-indexed
template<class DP, class EdgeData,
         DP(*merge)(DP, DP), DP(*e)(),
         DP(*putEdge)(DP, EdgeData), DP(*putVertex)(DP, int)>
struct Rerooting {
    struct Edge {
        int to;
        EdgeData data;
    };

    int n;
    vector<vector<Edge>> G;
    vector<DP> sub, answer;

    Rerooting(int N) : n(N), G(N), sub(N, e()), answer(N, e()) {}

    void addEdge(int u, int v, EdgeData data) {
        G[u].push_back({v, data});
        G[v].push_back({u, data});
    }
    DP dfs(int v, int parent) {
        DP value = e();
        for (auto edge : G[v]) {
            if (edge.to == parent) continue;
            value = merge(value, putEdge(dfs(edge.to, v), edge.data));
        }
        return sub[v] = putVertex(value, v);
    }
    void reroot(int v, int parent, DP fromParent) {
        int m = (int)G[v].size();
        vector<DP> value(m), left(m + 1, e()), right(m + 1, e());
        for (int i = 0; i < m; i++) {
            auto edge = G[v][i];
            DP x = edge.to == parent ? fromParent : sub[edge.to];
            value[i] = putEdge(x, edge.data);
            left[i + 1] = merge(left[i], value[i]);
        }
        for (int i = m - 1; i >= 0; i--) right[i] = merge(value[i], right[i + 1]);
        answer[v] = putVertex(left[m], v);
        for (int i = 0; i < m; i++) {
            int to = G[v][i].to;
            if (to == parent) continue;
            DP withoutChild = merge(left[i], right[i + 1]);
            reroot(to, v, putVertex(withoutChild, v));
        }
    }
    vector<DP> run(int root = 0) {
        dfs(root, -1);
        reroot(root, -1, e());
        return answer;
    }
};

/*
使用例: 各頂点から全頂点までの距離の総和

struct DP { ll sum, count; };
DP merge(DP a, DP b) { return {a.sum + b.sum, a.count + b.count}; }
DP e() { return {0, 0}; }
DP putEdge(DP x, ll cost) { x.sum += x.count * cost; return x; }
DP putVertex(DP x, int v) { x.count++; return x; }

Rerooting<DP, ll, merge, e, putEdge, putVertex> tree(N);
tree.addEdge(u, v, cost);
vector<DP> answer = tree.run();

putEdge  : 隣の頂点で完成した DP を、辺を越えて現在の頂点側へ移す
putVertex: 全ての隣接部分木を merge した後、現在の頂点自身を加える
*/

// 全方位木 DP
// putEdge(x, cost) は隣の頂点で完成した DP を辺越しに変換する
// putVertex(x, v) は子からの値をすべて merge した後、頂点 v を加える
// O(N)、添字は 0-indexed
// 使い方:
// DP と merge/e/putEdge/putVertex を定義し、Rerooting<...> tree(N) を作る。
// tree.addEdge(u, v, data) で無向辺を追加し、vector<DP> answer = tree.run(root);
// answer[v] は v を根とした木全体の DP。merge は結合的で e が単位元であること。
// 距離和を求める DP と各関数の具体例はファイル末尾のコメントを参照する。
// 使いどころ: 各頂点を根としたときの答えを、隣接部分木の情報を結合して全頂点分求める場合。
// 具体例: パス 0-1-2 で各頂点から全頂点への距離和を求めると answer={3, 2, 3}。
// sub[v] は仮根で見た子側だけの DP、answer[v] は親側も含む木全体の DP。
// merge の引数順は隣接リスト順。非可換 DP でも前後累積積により順序を保つ。
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

    // N 頂点の空の木と、各頂点用の DP 配列を単位元で初期化する。
    Rerooting(int N) : n(N), G(N), sub(N, e()), answer(N, e()) {}

    // データ data を持つ無向辺 u-v を追加する。
    void addEdge(int u, int v, EdgeData data) {
        G[u].push_back({v, data});
        G[v].push_back({u, data});
    }
    // v を根とする子方向の DP を後行順に求め、sub[v] を返す。
    DP dfs(int v, int parent) {
        DP value = e();
        for (auto edge : G[v]) {
            if (edge.to == parent) continue;
            value = merge(value, putEdge(dfs(edge.to, v), edge.data));
        }
        return sub[v] = putVertex(value, v);
    }
    // 親側 DP を受け取り、前後累積積で各子へ渡す DP と answer[v] を求める。
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
    // root を仮根として二回の DFS を行い、各頂点を根とした DP を返す。
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

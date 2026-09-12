// 無向グラフの補グラフの連結成分。set を用いて O((N + M) log N)
// G は自己辺なしを仮定する
// 使い方:
// 元グラフの無向隣接リスト G を渡して auto groups = complementGraphComponents(G);
// 辺を実際に補集合へ展開する必要はない。groups の各 vector が補グラフの連結成分。
// G[u] と G[v] には無向辺を両方向に入れる。多重辺は避ける。
// 使いどころ: 「元グラフで辺がない二頂点は移動できる」という関係の連結成分を求める場合。
// 具体例: 3 頂点の元グラフが辺 0-1 だけなら、補グラフには 0-2, 1-2 があり全頂点が同一成分。
// 補グラフの辺数は O(N^2) になり得るが、この関数は補辺を列挙しない。
// 無向グラフ G の非辺を辺とみなした補グラフについて、頂点集合ごとの連結成分を返す。
vector<vector<int>> complementGraphComponents(const vector<vector<int>>& G) {
    int N = (int)G.size();
    set<int> unused;
    for (int v = 0; v < N; v++) unused.insert(v);
    vector<int> mark(N, -1);
    vector<vector<int>> groups;
    while (!unused.empty()) {
        int s = *unused.begin();
        unused.erase(s);
        groups.push_back({});
        queue<int> que;
        que.push(s);
        while (!que.empty()) {
            int v = que.front(); que.pop();
            groups.back().push_back(v);
            for (int to : G[v]) mark[to] = v;
            for (auto it = unused.begin(); it != unused.end();) {
                int to = *it;
                if (mark[to] != v) {
                    que.push(to);
                    it = unused.erase(it);
                } else {
                    it++;
                }
            }
        }
    }
    return groups;
}

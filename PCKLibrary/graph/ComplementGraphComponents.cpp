//無向グラフの補グラフの連結成分。set を用いて O((N + M) log N)
//G は自己辺なしを仮定する
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

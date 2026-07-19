//トポロジカルソート
//入次数が0の頂点が最初に来る
bool topologicalSort(const vector<vector<int>>& G, vector<int>& res) {
    assert(res.empty());
    int V = (int)G.size();
    vector<int> deg(V);
    rep(i, 0, V) {
        for (int to : G[i]) deg[to]++;
    }
    queue<int> Q;
    rep(i, 0, V) {
        if (deg[i] == 0) Q.push(i);
    }
    while(!Q.empty()) {
        int v = Q.front(); Q.pop();
        res.push_back(v);
        for (int to : G[v]) {
            deg[to]--;
            if (deg[to] == 0) Q.push(to);
        }
    }
    if ((int)res.size() != V) {
        res.clear();
        return false;
    }
    return true;
}
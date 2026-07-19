//DAG判定
bool isDAG(const vector<vector<int>>& G) {
    int V = (int)G.size();
    vector<int> deg(V, 0);
    rep(i, 0, V) {
        for (int to : G[i]) deg[to]++;
    }
    queue<int> Q;
    rep(i, 0, V) if (deg[i] == 0) Q.push(i);
    vector<bool> ans(V, false);
    while(!Q.empty()) {
        int v = Q.front(); Q.pop();
        ans[v] = true;
        for (int to : G[v]) {
            deg[to]--;
            if (deg[to] == 0) Q.push(to);
        }
    }
    bool k = true;
    rep(i, 0, V) if (!ans[i]) k = false;
    return k;
}
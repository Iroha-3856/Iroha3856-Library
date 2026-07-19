//幅優先探索
//重みなしグラフにおける最短経路問題
vector<int> bfs(const vector<vector<int>>  &G, int start, int impossible) {
    queue<int> Q;
    vector<int> dist((int)G.size(), impossible);
    Q.push(start);
    dist[start] = 0;
    while(!Q.empty()) {
        int pos = Q.front(); Q.pop();
        for (int to : G[pos]) {
            if (dist[to]!=impossible) continue;
            Q.push(to);
            dist[to] = dist[pos]+1;
        }
    }
    return dist;
}
//dijkstra
//重み付きグラフにおける最短経路問題
vector<long long> dijkstra(const vector<vector<Edge>>& G, int start, long long impossible) {
    int N = (int)G.size();
    vector<long long> dist(N, impossible);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> Q;
    dist[start] = 0LL;
    Q.emplace(dist[start], start);
    while(!Q.empty()) {
        pair<long long, int> pos = Q.top();
        Q.pop();
        long long d = pos.first;
        int v = pos.second;
        if (dist[v] != d) continue;
        for (Edge e : G[v]) {
            if (dist[e.to]==impossible || dist[e.to]>d+e.cost) {
                dist[e.to] = d+e.cost;
                Q.push(make_pair(dist[e.to], e.to));
            }
        }
    }
    return dist;
}
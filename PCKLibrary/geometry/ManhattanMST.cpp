//依存: geometry/Geometry2D_integer.cpp
//Manhattan MST に十分な O(N) 本の候補辺を O(N log N) で列挙する
//返り値は {距離, u, v}。Kruskal に渡して使う
vector<tuple<ll, int, int>> manhattanMSTEdges(const vector<Point>& P) {
    int N = (int)P.size();
    vector<Point> Q = P;
    vector<int> id(N);
    vector<tuple<ll, int, int>> edges;
    for (int dir = 0; dir < 4; dir++) {
        iota(id.begin(), id.end(), 0);
        sort(id.begin(), id.end(), [&](int i, int j) {
            return Q[i].x + Q[i].y < Q[j].x + Q[j].y;
        });
        map<ll, int> sweep;
        for (int i : id) {
            for (auto it = sweep.lower_bound(-Q[i].y); it != sweep.end();) {
                int j = it->second;
                if (Q[i].x - Q[j].x < Q[i].y - Q[j].y) break;
                ll d = abs(P[i].x - P[j].x) + abs(P[i].y - P[j].y);
                edges.push_back({d, i, j});
                it = sweep.erase(it);
            }
            sweep[-Q[i].y] = i;
        }
        for (Point& p : Q) {
            if (dir & 1) p.x = -p.x;
            else swap(p.x, p.y);
        }
    }
    return edges;
}

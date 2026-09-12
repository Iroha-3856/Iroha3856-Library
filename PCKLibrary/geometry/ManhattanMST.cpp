// 依存: geometry/Geometry2D_integer.cpp
// Manhattan MST に十分な O(N) 本の候補辺を O(N log N) で列挙する
// 返り値は {距離, u, v}。Kruskal に渡して使う
// 使いどころ: 完全グラフの辺を O(N^2) 本作らず、L1 距離の最小全域木を求める場合。
// 具体例: P={(0, 0), (2, 1)} なら、元添字 0, 1 と Manhattan 距離 3 の候補辺が得られる。
// 使い方:
// auto edges = manhattanMSTEdges(points); を cost で sort し、Union-Find で Kruskal する。
// u, v は points の元添字、cost=|xu-xv|+|yu-yv|。候補には重複辺が含まれてよい。
// この関数は MST コストや辺集合そのものを返さない。N<=1 なら候補は空。
// 点集合 P の Manhattan MST を含む候補辺を {距離, 元添字u, v} で返す。
vector<tuple<ll, int, int>> manhattanMSTEdges(const vector<Point>& P) {
    int N = (int)P.size();
    vector<Point> Q = P;
    vector<int> id(N);
    vector<tuple<ll, int, int>> edges;
    for (int dir = 0; dir < 4; dir++) {
        iota(id.begin(), id.end(), 0);
        // 現在の座標変換で x+y の昇順に sweep する。
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

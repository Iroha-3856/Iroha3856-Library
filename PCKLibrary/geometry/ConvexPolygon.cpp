// 依存: geometry/Geometry2D_double.cpp
// 反時計回り凸多角形。点包含 O(log N)、直径 O(N)、cut / 接点列挙 O(N)
// 使いどころ: 凸包を作った後、点包含・最遠点対・直線による切断・外点からの接点を求める場合。
// 具体例: P={(0, 0), (2, 0), (2, 2), (0, 2)} では containsConvex(P, (1, 1))=2、(2, 1) なら 1。
// 使い方:
// P は反時計回りの凸多角形とする。containsConvex(P, p) は外部0・辺上1・内部2。
// convexDiameter(P) は {端点添字i, j, 距離}。P は空でないこと。
// convexCut(P, {a, b}) は有向直線 a->b の左側を残し、新しい反時計回り多角形を返す。
// tangentVertices(P, p) は外点 p からの支持線が触れる頂点番号を返し、非狭義凸では複数あり得る。
// 反時計回り凸多角形 P に対する p の位置を外0・辺上1・内部2で返す。
int containsConvex(const Polygon& P, Point p) {
    int N = (int)P.size();
    if (N == 0) return 0;
    if (N == 1) return abs(P[0] - p) < EPS;
    if (N == 2) return onSegment(P[0], P[1], p);
    int a = ccw(P[0], P[1], p), b = ccw(P[0], P[N - 1], p);
    if (a < 0 or b > 0) return 0;
    int l = 1, r = N - 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (ccw(P[0], P[m], p) >= 0) l = m;
        else r = m;
    }
    if (onSegment(P[0], P[1], p) or onSegment(P[0], P[N - 1], p)
        or onSegment(P[l], P[r], p)) return 1;
    return ccw(P[l], P[r], p) > 0 ? 2 : 0;
}

// 凸多角形 P の直径を実現する {頂点i, 頂点j, 距離} を返す。
tuple<int, int, long double> convexDiameter(const Polygon& P) {
    int N = (int)P.size();
    assert(N > 0);
    if (N == 1) return {0, 0, 0};
    int j = 1, bi = 0, bj = 1;
    long double best = norm(P[0] - P[1]);
    for (int i = 0; i < N; i++) {
        int ni = (i + 1) % N;
        while (cross(P[ni] - P[i], P[(j + 1) % N] - P[j]) > EPS) j = (j + 1) % N;
        if (chmax(best, norm(P[i] - P[j]))) bi = i, bj = j;
        if (chmax(best, norm(P[ni] - P[j]))) bi = ni, bj = j;
    }
    return {bi, bj, sqrtl(best)};
}

// 有向直線 l.a->l.b の左閉半平面と凸多角形 P の共通部分を返す。
Polygon convexCut(const Polygon& P, Line l) {
    Polygon ret;
    for (int i = 0; i < (int)P.size(); i++) {
        Point a = P[i], b = P[(i + 1) % P.size()];
        long double x = cross(l.b - l.a, a - l.a);
        long double y = cross(l.b - l.a, b - l.a);
        if (x >= -EPS) ret.push_back(a);
        if (sgn(x) * sgn(y) < 0) ret.push_back(a + (b - a) * x / (x - y));
    }
    return ret;
}

// 外点 p から P へ引いた支持線が接する頂点番号を返す。
vector<int> tangentVertices(const Polygon& P, Point p) {
    vector<int> ret;
    int N = (int)P.size();
    for (int i = 0; i < N; i++) {
        long double a = cross(P[i] - p, P[(i + N - 1) % N] - p);
        long double b = cross(P[i] - p, P[(i + 1) % N] - p);
        if (a * b >= -EPS) ret.push_back(i);
    }
    return ret;
}

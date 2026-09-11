//依存: geometry/Geometry2D_double.cpp
//反時計回り凸多角形。点包含 O(log N)、直径 O(N)、cut / 接点列挙 O(N)
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

//有向直線 l.a -> l.b の左側を残す
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

//外点 p から引いた支持線が接する頂点。非狭義凸では複数返りうる
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

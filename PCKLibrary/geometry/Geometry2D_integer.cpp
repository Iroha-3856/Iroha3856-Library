// 整数幾何。内積・外積が long long に収まることを仮定する
// 使いどころ: 格子点だけを扱い、EPS なしで向き・交差・包含・Pick の定理を厳密判定したい場合。
// 具体例: signedArea2({(0, 0), (3, 0), (0, 2)})=6。反時計回りなので符号は正、面積は 3。
// 使い方:
// Point p(x, y), Polygon P; とし、dot/cross/ccw/onSegment/intersectSS を厳密判定に使う。
// contains(P, p) は外部0・辺上1・内部2、signedArea2(P) は符号付き面積の2倍。
// convexHull(P, keepCollinear) は反時計回り。latticePoints(P) は {内部点, 境界点}。
// latticePoints は単純格子多角形を仮定する。double 版とは型名が衝突するため同時に使わない。
struct Point {
    ll x, y;
    // 整数座標 (X, Y) の点・ベクトルを作る。
    Point(ll X = 0, ll Y = 0) : x(X), y(Y) {}
    // ベクトル p を自身へ加える。
    Point& operator+=(Point p) { x += p.x; y += p.y; return *this; }
    // ベクトル p を自身から引く。
    Point& operator-=(Point p) { x -= p.x; y -= p.y; return *this; }
    // 二点・ベクトルの成分ごとの和を返す。
    friend Point operator+(Point p, Point q) { return p += q; }
    // 二点・ベクトルの成分ごとの差を返す。
    friend Point operator-(Point p, Point q) { return p -= q; }
    // 二点の座標が等しいかを返す。
    friend bool operator==(Point p, Point q) { return p.x == q.x and p.y == q.y; }
    // x、次に y の辞書順で大小を比較する。
    friend bool operator<(Point p, Point q) { return p.x != q.x ? p.x < q.x : p.y < q.y; }
};
using Polygon = vector<Point>;

// ベクトル a, b の内積を返す。
ll dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
// ベクトル a, b の外積を返す。
ll cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
// 有向点列 a->b->c の向きを左折1・共線0・右折-1で返す。
int ccw(Point a, Point b, Point c) {
    ll x = cross(b - a, c - a);
    return (x > 0) - (x < 0);
}
// 点 p が閉線分 ab 上にあるかを厳密に判定する。
bool onSegment(Point a, Point b, Point p) {
    return ccw(a, b, p) == 0 and dot(p - a, p - b) <= 0;
}
// 二つの閉線分 ab, cd が端点・重なりを含め交差するかを返す。
bool intersectSS(Point a, Point b, Point c, Point d) {
    int x = ccw(a, b, c), y = ccw(a, b, d);
    int z = ccw(c, d, a), w = ccw(c, d, b);
    if (x == 0 and onSegment(a, b, c)) return true;
    if (y == 0 and onSegment(a, b, d)) return true;
    if (z == 0 and onSegment(c, d, a)) return true;
    if (w == 0 and onSegment(c, d, b)) return true;
    return x * y < 0 and z * w < 0;
}

// 多角形 P の符号付き面積の 2 倍を返す。反時計回りなら正。
ll signedArea2(const Polygon& P) {
    ll ret = 0;
    for (int i = 0; i < (int)P.size(); i++) ret += cross(P[i], P[(i + 1) % P.size()]);
    return ret;
}
// P が面積を持つ凸多角形かを返す。頂点順は時計・反時計のどちらでもよい。
bool isConvex(const Polygon& P) {
    int direction = 0;
    for (int i = 0; i < (int)P.size(); i++) {
        int d = ccw(P[i], P[(i + 1) % P.size()], P[(i + 2) % P.size()]);
        if (d and direction and d != direction) return false;
        if (d) direction = d;
    }
    return direction != 0;
}
// 単純多角形 P に対する p の位置を外部0・辺上1・内部2で返す。
int contains(const Polygon& P, Point p) {
    bool in = false;
    for (int i = 0; i < (int)P.size(); i++) {
        Point a = P[i] - p, b = P[(i + 1) % P.size()] - p;
        if (onSegment(a, b, Point())) return 1;
        if (a.y > b.y) swap(a, b);
        if (a.y <= 0 and b.y > 0 and cross(a, b) > 0) in = !in;
    }
    return in ? 2 : 0;
}

// 点集合 P の反時計回り凸包を返す。keepCollinear で境界上の点を残す。
Polygon convexHull(Polygon P, bool keepCollinear = false) {
    sort(P.begin(), P.end());
    P.erase(unique(P.begin(), P.end()), P.end());
    if (P.size() <= 2) return P;
    bool allCollinear = true;
    for (int i = 2; i < (int)P.size(); i++) if (ccw(P[0], P[1], P[i])) allCollinear = false;
    if (allCollinear) return keepCollinear ? P : Polygon{P.front(), P.back()};
    Polygon lower, upper;
    for (Point p : P) {
        while (lower.size() >= 2) {
            int d = ccw(lower[lower.size() - 2], lower.back(), p);
            if (d > 0 or (keepCollinear and d == 0)) break;
            lower.pop_back();
        }
        lower.push_back(p);
    }
    reverse(P.begin(), P.end());
    for (Point p : P) {
        while (upper.size() >= 2) {
            int d = ccw(upper[upper.size() - 2], upper.back(), p);
            if (d > 0 or (keepCollinear and d == 0)) break;
            upper.pop_back();
        }
        upper.push_back(p);
    }
    lower.pop_back(); upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

// 単純格子多角形 P の {内部格子点数, 境界格子点数} を Pick の定理で返す。
pair<ll, ll> latticePoints(const Polygon& P) {
    ll area2 = abs(signedArea2(P)), boundary = 0;
    for (int i = 0; i < (int)P.size(); i++) {
        Point d = P[i] - P[(i + 1) % P.size()];
        boundary += gcd(abs(d.x), abs(d.y));
    }
    return {(area2 - boundary + 2) / 2, boundary};
}

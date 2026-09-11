//整数幾何。内積・外積が long long に収まることを仮定する
struct Point {
    ll x, y;
    Point(ll X = 0, ll Y = 0) : x(X), y(Y) {}
    Point& operator+=(Point p) { x += p.x; y += p.y; return *this; }
    Point& operator-=(Point p) { x -= p.x; y -= p.y; return *this; }
    friend Point operator+(Point p, Point q) { return p += q; }
    friend Point operator-(Point p, Point q) { return p -= q; }
    friend bool operator==(Point p, Point q) { return p.x == q.x and p.y == q.y; }
    friend bool operator<(Point p, Point q) { return p.x != q.x ? p.x < q.x : p.y < q.y; }
};
using Polygon = vector<Point>;

ll dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
ll cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
int ccw(Point a, Point b, Point c) {
    ll x = cross(b - a, c - a);
    return (x > 0) - (x < 0);
}
bool onSegment(Point a, Point b, Point p) {
    return ccw(a, b, p) == 0 and dot(p - a, p - b) <= 0;
}
bool intersectSS(Point a, Point b, Point c, Point d) {
    int x = ccw(a, b, c), y = ccw(a, b, d);
    int z = ccw(c, d, a), w = ccw(c, d, b);
    if (x == 0 and onSegment(a, b, c)) return true;
    if (y == 0 and onSegment(a, b, d)) return true;
    if (z == 0 and onSegment(c, d, a)) return true;
    if (w == 0 and onSegment(c, d, b)) return true;
    return x * y < 0 and z * w < 0;
}

//面積の 2 倍。反時計回りなら正
ll signedArea2(const Polygon& P) {
    ll ret = 0;
    for (int i = 0; i < (int)P.size(); i++) ret += cross(P[i], P[(i + 1) % P.size()]);
    return ret;
}
bool isConvex(const Polygon& P) {
    int direction = 0;
    for (int i = 0; i < (int)P.size(); i++) {
        int d = ccw(P[i], P[(i + 1) % P.size()], P[(i + 2) % P.size()]);
        if (d and direction and d != direction) return false;
        if (d) direction = d;
    }
    return direction != 0;
}
//0: 外部、1: 辺上、2: 内部
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

//反時計回りの凸包。O(NlogN)
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

//格子多角形の {内部格子点数, 境界格子点数}（Pick の定理）
pair<ll, ll> latticePoints(const Polygon& P) {
    ll area2 = abs(signedArea2(P)), boundary = 0;
    for (int i = 0; i < (int)P.size(); i++) {
        Point d = P[i] - P[(i + 1) % P.size()];
        boundary += gcd(abs(d.x), abs(d.y));
    }
    return {(area2 - boundary + 2) / 2, boundary};
}

// 実数幾何。Point(x, y)、x 座標は real(p)、y 座標は imag(p)
// 使いどころ: 交点や距離が整数にならない、直線・円・多角形を扱う標準的な二次元幾何。
// 具体例: ccw((0, 0), (1, 0), (0, 1))=1。x 軸正方向から上側へ曲がるので左折である。
// 使い方:
// Point p(x, y), Polygon P; とし、ベクトル演算は complex の +, -, *, /, abs, norm を使う。
// ccw(a, b, c) は左折 1、一直線 0、右折 -1。intersectSS は端点・退化線分を含む。
// distanceLP/SP/SS は直線・線分との距離、crosspointLL/SS/CL/CC は交点列を返す。
// contains(P, p) は外部0・辺上1・内部2。convexHull(P, keepCollinear) は反時計回り。
// EPS を使う実数版なので integer 版とは同時に写経せず、厳密な格子問題には integer 版を使う。
using Point = complex<long double>;
using Polygon = vector<Point>;
const long double EPS = 1e-10L;

// ベクトル a, b の内積を返す。
long double dot(Point a, Point b) { return real(conj(a) * b); }
// ベクトル a, b の外積を返す。
long double cross(Point a, Point b) { return imag(conj(a) * b); }
// EPS を零とみなし x の符号を -1, 0, 1 で返す。
int sgn(long double x) { return (x > EPS) - (x < -EPS); }
// 有向点列 a->b->c の向きを左折1・共線0・右折-1で返す。
int ccw(Point a, Point b, Point c) { return sgn(cross(b - a, c - a)); }
// ベクトル p を原点中心に theta rad だけ反時計回りへ回転する。
Point rotate(Point p, long double theta) { return p * polar(1.0L, theta); }

// 点 p が閉線分 ab 上にあるかを EPS 付きで判定する。
bool onSegment(Point a, Point b, Point p) {
    return ccw(a, b, p) == 0 and dot(p - a, p - b) <= EPS;
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

struct Line {
    Point a, b;
};
using Segment = Line;
struct Circle {
    Point center;
    long double radius;
};

// 点 p を非退化直線 l へ正射影した点を返す。
Point projection(Line l, Point p) {
    Point d = l.b - l.a;
    return l.a + d * dot(p - l.a, d) / norm(d);
}
// 非退化直線 l に関する点 p の鏡映を返す。
Point reflection(Line l, Point p) {
    return 2.0L * projection(l, p) - p;
}
// 非退化直線 l と点 p の Euclid 距離を返す。
long double distanceLP(Line l, Point p) {
    return abs(p - projection(l, p));
}
// 閉線分 s と点 p の Euclid 距離を返す。退化線分にも対応する。
long double distanceSP(Segment s, Point p) {
    if (abs(s.a - s.b) < EPS) return abs(p - s.a);
    if (dot(s.b - s.a, p - s.a) < 0) return abs(p - s.a);
    if (dot(s.a - s.b, p - s.b) < 0) return abs(p - s.b);
    return distanceLP(s, p);
}
// 二つの閉線分 s, t の Euclid 距離を返す。
long double distanceSS(Segment s, Segment t) {
    if (intersectSS(s.a, s.b, t.a, t.b)) return 0;
    return min({distanceSP(s, t.a), distanceSP(s, t.b),
                distanceSP(t, s.a), distanceSP(t, s.b)});
}

// 二直線 l, m の交点を返す。平行または一致する場合は空。
vector<Point> crosspointLL(Line l, Line m) {
    long double d = cross(l.b - l.a, m.b - m.a);
    if (sgn(d) == 0) return {};
    return {l.a + (l.b - l.a) * cross(m.a - l.a, m.b - m.a) / d};
}
// 二線分 s, t の交点を返す。重なる場合は共通部分の相異なる端点。
vector<Point> crosspointSS(Segment s, Segment t) {
    if (!intersectSS(s.a, s.b, t.a, t.b)) return {};
    vector<Point> ret = crosspointLL(s, t);
    if (!ret.empty()) return ret;
    for (Point p : {s.a, s.b, t.a, t.b}) {
        if (onSegment(s.a, s.b, p) and onSegment(t.a, t.b, p)) ret.push_back(p);
    }
    // 一致線分の共通端点を x、次に y の順へ並べる。
    sort(ret.begin(), ret.end(), [](Point a, Point b) {
        return real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b);
    });
    // EPS 未満しか離れていない交点を同一点として除く。
    ret.erase(unique(ret.begin(), ret.end(), [](Point a, Point b) {
        return abs(a - b) < EPS;
    }), ret.end());
    return ret;
}
// 円 c と非退化直線 l の交点を 0～2 個返す。
vector<Point> crosspointCL(Circle c, Line l) {
    Point p = projection(l, c.center);
    long double h2 = c.radius * c.radius - norm(p - c.center);
    if (h2 < -EPS) return {};
    if (h2 < EPS) return {p};
    Point d = (l.b - l.a) / abs(l.b - l.a) * sqrtl(h2);
    return {p - d, p + d};
}
// 二円 a, b の交点を 0～2 個返す。一致円では交点を列挙せず空。
vector<Point> crosspointCC(Circle a, Circle b) {
    long double d = abs(b.center - a.center);
    if (d < EPS or d > a.radius + b.radius + EPS or
        d < abs(a.radius - b.radius) - EPS) return {};
    long double x = (a.radius * a.radius - b.radius * b.radius + d * d) / (2 * d);
    long double h2 = a.radius * a.radius - x * x;
    Point p = a.center + (b.center - a.center) * x / d;
    if (h2 < EPS) return {p};
    Point q = (b.center - a.center) * Point(0, 1) * sqrtl(h2) / d;
    return {p - q, p + q};
}

// 多角形 P の符号付き面積を返す。反時計回りなら正。
long double signedArea(const Polygon& P) {
    long double ret = 0;
    for (int i = 0; i < (int)P.size(); i++) ret += cross(P[i], P[(i + 1) % P.size()]);
    return ret / 2;
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
        if (onSegment(a, b, 0)) return 1;
        if (imag(a) > imag(b)) swap(a, b);
        if (imag(a) <= 0 and imag(b) > 0 and cross(a, b) > 0) in = !in;
    }
    return in ? 2 : 0;
}

// 点集合 P の反時計回り凸包を返す。keepCollinear で境界上の点を残す。
Polygon convexHull(Polygon P, bool keepCollinear = false) {
    // Andrew 法の走査順として x、次に y の順へ整列する。
    sort(P.begin(), P.end(), [](Point a, Point b) {
        return real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b);
    });
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

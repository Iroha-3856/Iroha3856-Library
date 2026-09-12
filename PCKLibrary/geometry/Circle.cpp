// 依存: geometry/Geometry2D_double.cpp
// 三角形の外接円・内接円、円の共通接線、円の共通部分面積
// 使いどころ: 三点から円を復元する問題、二円の位置関係・接線・重なり面積を求める場合。
// 具体例: (0, 0), (2, 0), (0, 2) の circumcircle は中心 (1, 1)、半径 sqrt(2)。
// 使い方:
// circumcircle(a, b, c) / incircle(a, b, c) は退化三角形なら nullopt、そうでなければ Circle。
// commonTangents(c1, c2) は各共通接線を接点二つの Line として返す。同心円では空。
// circleIntersectionArea(c1, c2) は二円の共通部分面積。接するだけなら 0。
// auto circle=circumcircle(a, b, d); if (circle) cout << real(circle->center) << ' ' << circle->radius;
// 非共線な三点 a, b, c の外接円を返す。共線なら nullopt。
optional<Circle> circumcircle(Point a, Point b, Point c) {
    long double d = 2 * cross(b - a, c - a);
    if (abs(d) < EPS) return nullopt;
    Point x = b - a, y = c - a;
    Point center = a + Point(norm(x) * imag(y) - norm(y) * imag(x),
                             real(x) * norm(y) - real(y) * norm(x)) / d;
    return Circle{center, abs(center - a)};
}

// 非退化三角形 abc の内接円を返す。面積 0 なら nullopt。
optional<Circle> incircle(Point a, Point b, Point c) {
    long double x = abs(b - c), y = abs(c - a), z = abs(a - b);
    if (x + y + z < EPS or abs(cross(b - a, c - a)) < EPS) return nullopt;
    Point center = (x * a + y * b + z * c) / (x + y + z);
    return Circle{center, distanceLP({a, b}, center)};
}

// 二円の全共通接線を、各円上の接点を結ぶ Line として返す。
vector<Line> commonTangents(Circle a, Circle b) {
    vector<Line> ret;
    Point d = b.center - a.center;
    long double z = norm(d);
    if (z < EPS) return ret;
    for (int sign : {-1, 1}) {
        long double r = a.radius - sign * b.radius;
        long double h2 = z - r * r;
        if (h2 < -EPS) continue;
        h2 = max((long double)0, h2);
        for (int side : {-1, 1}) {
            Point v = (d * r + d * Point(0, side) * sqrtl(h2)) / z;
            ret.push_back({a.center + v * a.radius,
                           b.center + v * (sign * b.radius)});
            if (h2 < EPS) break;
        }
    }
    return ret;
}

// 二つの閉円板 a, b の共通部分面積を返す。
long double circleIntersectionArea(Circle a, Circle b) {
    long double d = abs(a.center - b.center);
    if (a.radius + b.radius <= d + EPS) return 0;
    if (d <= abs(a.radius - b.radius) + EPS) {
        long double r = min(a.radius, b.radius);
        return acosl(-1.0L) * r * r;
    }
    long double x = (d * d + a.radius * a.radius - b.radius * b.radius)
                  / (2 * d * a.radius);
    long double y = (d * d + b.radius * b.radius - a.radius * a.radius)
                  / (2 * d * b.radius);
    x = max((long double)-1, min((long double)1, x));
    y = max((long double)-1, min((long double)1, y));
    long double A = 2 * acosl(x), B = 2 * acosl(y);
    return (A - sinl(A)) * a.radius * a.radius / 2
         + (B - sinl(B)) * b.radius * b.radius / 2;
}

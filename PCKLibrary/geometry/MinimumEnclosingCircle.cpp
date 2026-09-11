//依存: geometry/Geometry2D_double.cpp, geometry/Circle.cpp
//最小包含円。乱択 incremental、期待 O(N)。重複点・共線点に対応
Circle minimumEnclosingCircle(Polygon P) {
    if (P.empty()) return {{0, 0}, 0};
    mt19937 rng(712367821);
    shuffle(P.begin(), P.end(), rng);
    Circle c{P[0], 0};
    auto outside = [&](Point p) { return abs(p - c.center) > c.radius + EPS; };
    for (int i = 0; i < (int)P.size(); i++) if (outside(P[i])) {
        c = {P[i], 0};
        for (int j = 0; j < i; j++) if (outside(P[j])) {
            c = {(P[i] + P[j]) / 2.0L, abs(P[i] - P[j]) / 2};
            for (int k = 0; k < j; k++) if (outside(P[k])) {
                optional<Circle> d = circumcircle(P[i], P[j], P[k]);
                if (d) c = *d;
                else {
                    pair<Point, Point> far = {P[i], P[j]};
                    if (abs(far.first - far.second) < abs(P[i] - P[k])) far = {P[i], P[k]};
                    if (abs(far.first - far.second) < abs(P[j] - P[k])) far = {P[j], P[k]};
                    c = {(far.first + far.second) / 2.0L, abs(far.first - far.second) / 2};
                }
            }
        }
    }
    return c;
}

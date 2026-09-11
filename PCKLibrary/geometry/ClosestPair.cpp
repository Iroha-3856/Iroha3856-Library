//依存: geometry/Geometry2D_double.cpp
//最近点対。{距離, 点1, 点2}、点が 2 個未満なら距離 INF。O(N log N)
tuple<long double, Point, Point> closestPair(Polygon P) {
    sort(P.begin(), P.end(), [](Point a, Point b) {
        return real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b);
    });
    Point bestA, bestB;
    long double answer = INF;
    function<long double(int, int)> dfs = [&](int l, int r) {
        if (r - l <= 1) return (long double)INF;
        int m = (l + r) / 2;
        long double x = real(P[m]);
        long double d = min(dfs(l, m), dfs(m, r));
        inplace_merge(P.begin() + l, P.begin() + m, P.begin() + r,
                      [](Point a, Point b) { return imag(a) < imag(b); });
        vector<Point> strip;
        for (int i = l; i < r; i++) if (abs(real(P[i]) - x) < d) {
            for (int j = (int)strip.size() - 1; j >= 0 and imag(P[i]) - imag(strip[j]) < d; j--) {
                long double nd = abs(P[i] - strip[j]);
                if (nd < d) d = nd;
                if (nd < answer) answer = nd, bestA = P[i], bestB = strip[j];
            }
            strip.push_back(P[i]);
        }
        return d;
    };
    dfs(0, (int)P.size());
    return {answer, bestA, bestB};
}

// 依存: geometry/Geometry2D_double.cpp
// 最近点対。{距離, 点1, 点2}、点が 2 個未満なら距離 INF。O(N log N)
// 使いどころ: 全点対 O(N^2) を避け、Euclid 距離が最小の二点と距離を同時に求める場合。
// 具体例: {(0, 0), (3, 4), (1, 0)} の最近距離は 1 で、対応する点は (0, 0), (1, 0)。
// 使い方:
// auto [distance, a, b] = closestPair(points); とすると a, b が最近点対。
// points は値渡しなので入力順を壊さない。重複点があれば距離 0。
// 2 点未満では distance==INF だけを確認し、a, b は利用しない。
// 点集合 P の最近二点について {Euclid距離, 点1, 点2} を返す。
tuple<long double, Point, Point> closestPair(Polygon P) {
    // 分割統治の前提として点を x、次に y の順へ並べる。
    sort(P.begin(), P.end(), [](Point a, Point b) {
        return real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b);
    });
    Point bestA, bestB;
    long double answer = INF;
    // x 座標順区間 [l, r) の最近距離を求め、区間を y 座標順へ inplace merge する。
    function<long double(int, int)> dfs = [&](int l, int r) {
        if (r - l <= 1) return (long double)INF;
        int m = (l + r) / 2;
        long double x = real(P[m]);
        long double d = min(dfs(l, m), dfs(m, r));
        // 再帰後の二つの y 順区間を一つの y 順区間へ併合する。
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

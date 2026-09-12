// 依存: geometry/Geometry2D_integer.cpp
// 反時計回り凸多角形の Minkowski sum。各入力に重複終点がないこと。O(N + M)
// 使いどころ: 二つの凸図形から一点ずつ選んだベクトル和全体を、凸多角形として構成する場合。
// 具体例: 一点集合 A={(1, 2)}, B={(3, 4)} の Minkowski 和は {(4, 6)}。
// 使い方:
// Polygon C = minkowskiSum(A, B); は {a+b | a in A, b in B} の凸多角形を返す。
// A, B は境界順の凸多角形で、先頭点を末尾に重ねない。時計回りでも内部で反転する。
// normalizeConvexPolygon(P) は向きと開始点だけを正規化し、非凸入力を凸包には直さない。
// 同方向の連続辺を持つ入力では、必要なら事前に凸包を取って冗長点を除く。
// 凸多角形 P を反時計回りにし、y 最小・次に x 最小の頂点を先頭へ移す。
Polygon normalizeConvexPolygon(Polygon P) {
    if (P.size() <= 1) return P;
    if (signedArea2(P) < 0) reverse(P.begin(), P.end());
    // 辺列 merge の始点となる y 最小・次に x 最小の頂点を選ぶ。
    int s = (int)(min_element(P.begin(), P.end(), [](Point a, Point b) {
        return a.y != b.y ? a.y < b.y : a.x < b.x;
    }) - P.begin());
    rotate(P.begin(), P.begin() + s, P.end());
    return P;
}

// 二つの凸多角形 A, B の Minkowski 和を境界順の凸多角形として返す。
Polygon minkowskiSum(Polygon A, Polygon B) {
    if (A.empty() or B.empty()) return {};
    A = normalizeConvexPolygon(A); B = normalizeConvexPolygon(B);
    if (A.size() == 1) {
        for (Point& p : B) p += A[0];
        return B;
    }
    if (B.size() == 1) {
        for (Point& p : A) p += B[0];
        return A;
    }
    vector<Point> ea(A.size()), eb(B.size());
    for (int i = 0; i < (int)A.size(); i++) ea[i] = A[(i + 1) % A.size()] - A[i];
    for (int i = 0; i < (int)B.size(); i++) eb[i] = B[(i + 1) % B.size()] - B[i];
    Polygon ret = {A[0] + B[0]};
    int i = 0, j = 0;
    while (i < (int)ea.size() or j < (int)eb.size()) {
        Point d;
        if (j == (int)eb.size() or (i < (int)ea.size() and cross(ea[i], eb[j]) > 0)) d = ea[i++];
        else if (i == (int)ea.size() or cross(ea[i], eb[j]) < 0) d = eb[j++];
        else d = ea[i++] + eb[j++];
        ret.push_back(ret.back() + d);
    }
    ret.pop_back();
    return ret;
}

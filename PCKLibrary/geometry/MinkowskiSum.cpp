//依存: geometry/Geometry2D_integer.cpp
//反時計回り凸多角形の Minkowski sum。各入力に重複終点がないこと。O(N + M)
Polygon normalizeConvexPolygon(Polygon P) {
    if (P.size() <= 1) return P;
    if (signedArea2(P) < 0) reverse(P.begin(), P.end());
    int s = (int)(min_element(P.begin(), P.end(), [](Point a, Point b) {
        return a.y != b.y ? a.y < b.y : a.x < b.x;
    }) - P.begin());
    rotate(P.begin(), P.begin() + s, P.end());
    return P;
}

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

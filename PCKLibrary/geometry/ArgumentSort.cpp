// 依存: geometry/Geometry2D_integer.cpp
// 整数ベクトルを偏角 [0, 2π) の昇順（正の x 軸から反時計回り）にソート。O(N log N)
// 使いどころ: atan2 の誤差を避けてベクトルを角度順に走査・グループ化したい場合。
// 使い方: auto Q = argumentSort({Point(0, 1), Point(1, 0), Point(-1, 0)});
// Q は {(1,0), (0,1), (-1,0)}。入力は変更せず、零ベクトルは先頭に置く。
// 同じ向きの非零ベクトルは原点からの距離の昇順。座標は外積・距離二乗が
// __int128_t に収まる範囲を仮定する。整数幾何と実数幾何は同時に使わない。
// ベクトル列 P を偏角でソートした新しい配列として返す。
vector<Point> argumentSort(vector<Point> P) {
    // 正の x 軸を含む上半平面を 0、それ以外を 1 とする。
    auto half = [](Point p) {
        return p.y > 0 or (p.y == 0 and p.x >= 0) ? 0 : 1;
    };
    // 上下半平面 -> 外積 -> 距離二乗の順に比較する。
    sort(P.begin(), P.end(), [&](Point a, Point b) {
        int ha = half(a), hb = half(b);
        if (ha != hb) return ha < hb;
        __int128_t c = (__int128_t)a.x * b.y - (__int128_t)a.y * b.x;
        if (c != 0) return c > 0;
        __int128_t na = (__int128_t)a.x * a.x + (__int128_t)a.y * a.y;
        __int128_t nb = (__int128_t)b.x * b.x + (__int128_t)b.y * b.y;
        return na < nb;
    });
    return P;
}

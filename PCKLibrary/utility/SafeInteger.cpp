// 符号付き整数の除算と、積を __int128_t で比較する補助関数
// b != 0。floorDiv / ceilDiv は数学的な床・天井を返す
// 使いどころ: 負数を含む座標の区間分割や、交差積の大小比較で ll の積を作りたくない場合。
// 具体例: floorDiv(-5, 2)=-3、ceilDiv(-5, 2)=-2。C++ の -5/2=-2 とは床の結果が異なる。
// 使い方:
// C++ の / が 0 方向へ丸める点を避けたいとき floorDiv(a, b), ceilDiv(a, b) を使う。
// productLess(a, b, c) は a*b<c、productLessEqual は a*b<=c を積の overflow なしで判定する。
// compareProduct(a, b, c, d) は a*b と c*d の大小を -1/0/1 で返す。引数と答えは ll の範囲。
template<class T>
// C++ の切捨て除算を補正し、数学的な floor(a/b) を返す。
T floorDiv(T a, T b) {
    assert(b != 0);
    T q = a / b, r = a % b;
    if (r != 0 and ((r < 0) != (b < 0))) q--;
    return q;
}

template<class T>
// C++ の切捨て除算を補正し、数学的な ceil(a/b) を返す。
T ceilDiv(T a, T b) {
    assert(b != 0);
    T q = a / b, r = a % b;
    if (r != 0 and ((r < 0) == (b < 0))) q++;
    return q;
}

// a*b<c を 128 bit 中間積で overflow せず判定する。
bool productLess(ll a, ll b, ll c) {
    return (__int128_t)a * b < c;
}

// a*b<=c を 128 bit 中間積で overflow せず判定する。
bool productLessEqual(ll a, ll b, ll c) {
    return (__int128_t)a * b <= c;
}

// a*b と c*d の大小をそれぞれ -1, 0, 1 で返す。
int compareProduct(ll a, ll b, ll c, ll d) {
    __int128_t x = (__int128_t)a * b;
    __int128_t y = (__int128_t)c * d;
    return (x > y) - (x < y);
}

//符号付き整数の除算と、積を __int128_t で比較する補助関数
//b != 0。floorDiv / ceilDiv は数学的な床・天井を返す
template<class T>
T floorDiv(T a, T b) {
    assert(b != 0);
    T q = a / b, r = a % b;
    if (r != 0 and ((r < 0) != (b < 0))) q--;
    return q;
}

template<class T>
T ceilDiv(T a, T b) {
    assert(b != 0);
    T q = a / b, r = a % b;
    if (r != 0 and ((r < 0) == (b < 0))) q++;
    return q;
}

bool productLess(ll a, ll b, ll c) {
    return (__int128_t)a * b < c;
}

bool productLessEqual(ll a, ll b, ll c) {
    return (__int128_t)a * b <= c;
}

int compareProduct(ll a, ll b, ll c, ll d) {
    __int128_t x = (__int128_t)a * b;
    __int128_t y = (__int128_t)c * d;
    return (x > y) - (x < y);
}

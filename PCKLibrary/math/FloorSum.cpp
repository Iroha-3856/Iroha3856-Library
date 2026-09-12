// sum_{0 <= i < n} floor((a i + b) / m)。n >= 0, m > 0
// 負の a, b に対応。O(log m)、答えは ll に収まること
// 使いどころ: 格子点数え上げや、一次式を整数除算した値の総和を n に比例せず求める場合。
// 具体例: floorSum(5, 7, -3, 2)=-5。各項は 0, -1, -1, -1, -2 となる。
// 使い方:
// ll answer = floorSum(n, m, a, b); で i=0, ..., n-1 の floor((a*i+b)/m) を合計する。
// a, b が負でも数学的 floor で処理する。n==0 は 0。
// floorSumUnsigned は内部用で a, b>=0 の場合だけ直接呼べる。
// a, b>=0 の floor sum を Euclid 型の変形で求める内部関数。
ll floorSumUnsigned(ll n, ll m, ll a, ll b) {
    __int128_t ret = 0;
    while (true) {
        if (a >= m) ret += (__int128_t)n * (n - 1) / 2 * (a / m), a %= m;
        if (b >= m) ret += (__int128_t)n * (b / m), b %= m;
        __int128_t y = (__int128_t)a * n + b;
        if (y < m) return (ll)ret;
        n = (ll)(y / m);
        b = (ll)(y % m);
        swap(m, a);
    }
}

// 負の a, b も含め sum_{0<=i<n} floor((a*i+b)/m) を返す。
ll floorSum(ll n, ll m, ll a, ll b) {
    assert(n >= 0 and m > 0);
    __int128_t ret = 0;
    if (a < 0) {
        ll q = a / m - (a % m != 0);
        ret += (__int128_t)q * n * (n - 1) / 2;
        a -= q * m;
    }
    if (b < 0) {
        ll q = b / m - (b % m != 0);
        ret += (__int128_t)q * n;
        b -= q * m;
    }
    return (ll)(ret + floorSumUnsigned(n, m, a, b));
}

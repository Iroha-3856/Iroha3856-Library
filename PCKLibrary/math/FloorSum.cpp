//sum_{0 <= i < n} floor((a i + b) / m)。n >= 0, m > 0
//負の a, b に対応。O(log m)、答えは ll に収まること
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

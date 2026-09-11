//依存: math/ExtGCD.cpp
//a^x = b (mod m) の最小非負 x。解なしは -1。非互いに素でもよい
//Baby-step Giant-step、O(sqrt(m) log m) time / O(sqrt(m)) space
ll discreteLogarithm(ll a, ll b, ll m) {
    assert(m >= 1);
    a %= m; b %= m;
    if (a < 0) a += m;
    if (b < 0) b += m;
    if (m == 1 or b == 1) return 0;
    ll offset = 0, coefficient = 1;
    while (true) {
        ll g = gcd(a, m);
        if (g == 1) break;
        if (b == coefficient) return offset;
        if (b % g) return -1;
        b /= g; m /= g;
        coefficient = (ll)((__int128_t)coefficient * (a / g) % m);
        offset++;
    }
    ll n = 1;
    while ((__int128_t)n * n < m) n++;
    unordered_map<ll, ll> baby;
    ll cur = b;
    for (ll q = 0; q <= n; q++) {
        baby[cur] = q;
        cur = (ll)((__int128_t)cur * a % m);
    }
    ll an = 1;
    for (ll i = 0; i < n; i++) an = (ll)((__int128_t)an * a % m);
    cur = coefficient;
    for (ll p = 1; p <= n + 1; p++) {
        cur = (ll)((__int128_t)cur * an % m);
        auto it = baby.find(cur);
        if (it != baby.end()) return p * n - it->second + offset;
    }
    return -1;
}

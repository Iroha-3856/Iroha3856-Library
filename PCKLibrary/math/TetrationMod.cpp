//a ↑↑ height mod m。0^0 = 1、a >= 0, height >= 0, m >= 1
//Euler φ の鎖を用い、非互いに素な a, m にも対応。O(sqrt(m) log height)
ll eulerPhi(ll n) {
    ll ret = n;
    for (ll p = 2; p <= n / p; p++) if (n % p == 0) {
        ret = ret / p * (p - 1);
        while (n % p == 0) n /= p;
    }
    if (n > 1) ret = ret / n * (n - 1);
    return ret;
}

ll powerModTetration(ll a, ll n, ll mod) {
    ll ret = 1 % mod;
    a %= mod;
    while (n) {
        if (n & 1) ret = (ll)((__int128_t)ret * a % mod);
        a = (ll)((__int128_t)a * a % mod);
        n >>= 1;
    }
    return ret;
}

ll powerCapped(ll a, ll n, ll cap) {
    if (cap <= 1) return cap;
    __int128_t ret = 1, x = a;
    while (n) {
        if (n & 1) ret = min<__int128_t>(cap, ret * x);
        x = min<__int128_t>(cap, x * x);
        n >>= 1;
    }
    return (ll)ret;
}

ll towerCapped(ll a, ll height, ll cap) {
    if (cap <= 1) return cap;
    if (height == 0) return 1;
    if (height == 1) return min(a, cap);
    if (a == 0) return height % 2 == 0 ? 1 : 0;
    if (a == 1) return 1;
    ll need = 0, x = 1;
    while (x < cap) x = (ll)min<__int128_t>(cap, (__int128_t)x * a), need++;
    ll exponent = towerCapped(a, height - 1, need);
    return powerCapped(a, exponent, cap);
}

ll tetrationMod(ll a, ll height, ll mod) {
    assert(a >= 0 and height >= 0 and mod >= 1);
    if (mod == 1) return 0;
    if (height == 0) return 1 % mod;
    if (height == 1) return a % mod;
    ll phi = eulerPhi(mod);
    ll exponent = tetrationMod(a, height - 1, phi);
    if (towerCapped(a, height - 1, phi) == phi) exponent += phi;
    return powerModTetration(a, exponent, mod);
}

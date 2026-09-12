// a ↑↑ height mod m。0^0 = 1、a >= 0, height >= 0, m >= 1
// Euler φ の鎖を用い、非互いに素な a, m にも対応。O(sqrt(m) log height)
// 使いどころ: 指数にも同じ形の巨大な累乗塔が現れ、塔そのものを整数として保持できない場合。
// 具体例: tetrationMod(2, 3, 100)=16。高さ3は 2^(2^2) を表す。
// 使い方:
// ll value = tetrationMod(a, height, mod); で a^(a^(...)) mod mod を求める。
// height==0 は空の塔として 1、height==1 は a。0^0 は 1 とする規約。
// eulerPhi(n) は単体でも φ(n) を返す。中間塔の巨大さは capped 関数で判定し直接構築しない。
// 正整数 n の Euler φ(n) を試し割りで返す。
ll eulerPhi(ll n) {
    ll ret = n;
    for (ll p = 2; p <= n / p; p++) if (n % p == 0) {
        ret = ret / p * (p - 1);
        while (n % p == 0) n /= p;
    }
    if (n > 1) ret = ret / n * (n - 1);
    return ret;
}

// a^n mod mod を 128 bit 中間積を使う二分累乗で返す。
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

// min(a^n, cap) を 0^0=1 の規約で巨大な冪を生成せず返す。cap>=0。
ll powerCapped(ll a, ll n, ll cap) {
    assert(a >= 0 and n >= 0 and cap >= 0);
    if (cap == 0) return 0;
    if (n == 0) return 1;
    if (a == 0) return 0;
    if (cap == 1) return 1;
    __int128_t ret = 1, x = a;
    while (n) {
        if (n & 1) ret = min<__int128_t>(cap, ret * x);
        x = min<__int128_t>(cap, x * x);
        n >>= 1;
    }
    return (ll)ret;
}

// min(a↑↑height, cap) を返し、指数が十分大きいかの判定に使う。
ll towerCapped(ll a, ll height, ll cap) {
    if (cap == 0) return 0;
    if (height == 0) return 1;
    if (a == 0) return min(height % 2 == 0 ? 1LL : 0LL, cap);
    if (cap == 1) return 1;
    if (height == 1) return min(a, cap);
    if (a == 1) return 1;
    ll need = 0, x = 1;
    while (x < cap) x = (ll)min<__int128_t>(cap, (__int128_t)x * a), need++;
    ll exponent = towerCapped(a, height - 1, need);
    return powerCapped(a, exponent, cap);
}

// 0^0=1 の規約で a↑↑height mod mod を返す。
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

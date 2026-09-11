//依存: math/ExtGCD.cpp
//任意の正の mod に対する C(n, k)。O(k * ω(mod) log n + sqrt(mod))
//k が小さい場合用。各素因数を除いた分母だけを逆元にする
ll binomialArbitraryMod(ll n, ll k, ll mod) {
    assert(n >= 0 and mod > 0);
    if (k < 0 or n < k or mod == 1) return 0;
    k = min(k, n - k);
    vector<ll> primes;
    ll x = mod;
    for (ll p = 2; p <= x / p; p++) if (x % p == 0) {
        primes.push_back(p);
        while (x % p == 0) x /= p;
    }
    if (x > 1) primes.push_back(x);
    vector<ll> exponent(primes.size());
    ll ret = 1 % mod;
    for (ll i = 1; i <= k; i++) {
        ll a = n - k + i, b = i;
        for (int j = 0; j < (int)primes.size(); j++) {
            ll p = primes[j];
            while (a % p == 0) a /= p, exponent[j]++;
            while (b % p == 0) b /= p, exponent[j]--;
        }
        ret = (ll)((__int128_t)ret * (a % mod) % mod);
        ll inv, y;
        assert(extGCD(b, mod, inv, y) == 1);
        inv %= mod;
        if (inv < 0) inv += mod;
        ret = (ll)((__int128_t)ret * inv % mod);
    }
    for (int j = 0; j < (int)primes.size(); j++) {
        ll p = primes[j], e = exponent[j], a = p % mod;
        while (e) {
            if (e & 1) ret = (ll)((__int128_t)ret * a % mod);
            a = (ll)((__int128_t)a * a % mod);
            e >>= 1;
        }
    }
    return ret;
}

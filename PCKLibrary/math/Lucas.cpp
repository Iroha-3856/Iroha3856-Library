//素数 p に対する C(n, k) mod p。前計算 O(p)、query O(log_p n)
//p が配列を確保できる程度に小さい場合に使う
struct LucasBinomial {
    ll p;
    vector<ll> fact, invFact;

    ll modPow(ll a, ll n) const {
        ll ret = 1;
        while (n) {
            if (n & 1) ret = (ll)((__int128_t)ret * a % p);
            a = (ll)((__int128_t)a * a % p);
            n >>= 1;
        }
        return ret;
    }
    LucasBinomial(ll prime) : p(prime), fact(p), invFact(p) {
        assert(p >= 2);
        fact[0] = 1;
        for (ll i = 1; i < p; i++) fact[i] = fact[i - 1] * i % p;
        invFact[p - 1] = modPow(fact[p - 1], p - 2);
        for (ll i = p - 1; i; i--) invFact[i - 1] = invFact[i] * i % p;
    }
    ll small(ll n, ll k) const {
        if (k < 0 or n < k) return 0;
        return fact[n] * invFact[k] % p * invFact[n - k] % p;
    }
    ll binomial(ll n, ll k) const {
        if (k < 0 or n < k) return 0;
        ll ret = 1;
        while (n or k) {
            ret = ret * small(n % p, k % p) % p;
            n /= p; k /= p;
        }
        return ret;
    }
};

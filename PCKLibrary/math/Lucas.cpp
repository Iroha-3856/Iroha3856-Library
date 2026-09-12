// 素数 p に対する C(n, k) mod p。前計算 O(p)、query O(log_p n)
// p が配列を確保できる程度に小さい場合に使う
// 使いどころ: n, k は巨大だが法 p が小さい素数で、二項係数を p 進各桁へ分解できる場合。
// 具体例: LucasBinomial(7).binomial(10, 3)=1。C(10, 3)=120=1 mod 7 である。
// 使い方:
// LucasBinomial comb(p); を一度構築し、comb.binomial(n, k) で C(n, k) mod p を得る。
// n, k は p より大きくてよい。k<0 または k>n は 0。
// fact と invFact を長さ p で持つため、p が大きい場合には使わない。p は素数であること。
struct LucasBinomial {
    ll p;
    vector<ll> fact, invFact;

    // a^n mod p を二分累乗で返す。
    ll modPow(ll a, ll n) const {
        ll ret = 1;
        while (n) {
            if (n & 1) ret = (ll)((__int128_t)ret * a % p);
            a = (ll)((__int128_t)a * a % p);
            n >>= 1;
        }
        return ret;
    }
    // 素数 prime 未満の階乗と逆階乗を前計算する。
    LucasBinomial(ll prime) : p(prime), fact(p), invFact(p) {
        assert(p >= 2);
        fact[0] = 1;
        for (ll i = 1; i < p; i++) fact[i] = fact[i - 1] * i % p;
        invFact[p - 1] = modPow(fact[p - 1], p - 2);
        for (ll i = p - 1; i; i--) invFact[i - 1] = invFact[i] * i % p;
    }
    // 0<=n, k<p に対する C(n, k) mod p を返す。範囲外の k は 0。
    ll small(ll n, ll k) const {
        if (k < 0 or n < k) return 0;
        return fact[n] * invFact[k] % p * invFact[n - k] % p;
    }
    // Lucas の定理で任意の非負 n に対する C(n, k) mod p を返す。
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

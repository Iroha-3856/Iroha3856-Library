// 依存: math/NumberTheory64.cpp, math/DiscreteLogarithm.cpp, math/ExtGCD.cpp
// 以下は素数 p に対する原始根・平方根・k 乗根。解なしは -1
// 使いどころ: 素数 mod 上で x^2=a や x^k=a の解を一つ復元する場合。
// 具体例: modSqrtPrime(10, 13) は 6 または 7。どちらも二乗すると 10 mod 13 になる。
// 使い方:
// primitiveRootPrime(p) は素数 p の最小とは限らない原始根を一つ返す。
// modSqrtPrime(a, p) は x^2=a、discreteKthRootPrime(a, k, p) は x^k=a の解を一つ返す。
// 後二つは解なしなら -1。返った x は必ず式へ代入して検算でき、別解の列挙はしない。
// p は素数、k>=1。k 乗根は内部で離散対数を使うため O(sqrt(p)) の領域を要する。
// 素数 p の乗法群を生成する原始根を一つ返す。
ll primitiveRootPrime(ll p) {
    assert(p >= 2 and isPrime64(p));
    if (p == 2) return 1;
    ll phi = p - 1, x = phi;
    vector<ll> factors;
    for (ll q = 2; q <= x / q; q++) if (x % q == 0) {
        factors.push_back(q);
        while (x % q == 0) x /= q;
    }
    if (x > 1) factors.push_back(x);
    for (ll g = 2;; g++) {
        bool ok = true;
        for (ll q : factors) if (modPower64(g, phi / q, p) == 1) ok = false;
        if (ok) return g;
    }
}

// 素数 p に対して x^2=a mod p の解を一つ返し、存在しなければ -1。
ll modSqrtPrime(ll a, ll p) {
    assert(p >= 2 and isPrime64(p));
    a %= p;
    if (a < 0) a += p;
    if (a == 0 or p == 2) return a;
    if (modPower64(a, (p - 1) / 2, p) != 1) return -1;
    ll q = p - 1, s = 0;
    while ((q & 1) == 0) q >>= 1, s++;
    if (s == 1) return modPower64(a, (p + 1) / 4, p);
    ll z = 2;
    while (modPower64(z, (p - 1) / 2, p) != (u64)(p - 1)) z++;
    ll c = modPower64(z, q, p);
    ll x = modPower64(a, (q + 1) / 2, p);
    ll t = modPower64(a, q, p), m = s;
    while (t != 1) {
        ll i = 1, y = modMultiply64(t, t, p);
        while (y != 1) y = modMultiply64(y, y, p), i++;
        ll b = modPower64(c, 1LL << (m - i - 1), p);
        x = modMultiply64(x, b, p);
        c = modMultiply64(b, b, p);
        t = modMultiply64(t, c, p);
        m = i;
    }
    return x;
}

// 素数 p に対して x^k=a mod p の解を一つ返し、存在しなければ -1。
ll discreteKthRootPrime(ll a, ll k, ll p) {
    assert(k >= 1 and p >= 2 and isPrime64(p));
    a %= p;
    if (a < 0) a += p;
    if (a == 0) return 0;
    ll g = primitiveRootPrime(p);
    ll y = discreteLogarithm(g, a, p);
    ll d = gcd(k, p - 1);
    if (y % d) return -1;
    ll mod = (p - 1) / d, inv, z;
    extGCD(k / d, mod, inv, z);
    inv %= mod;
    if (inv < 0) inv += mod;
    ll exponent = (ll)((__int128_t)(y / d) * inv % mod);
    return modPower64(g, exponent, p);
}

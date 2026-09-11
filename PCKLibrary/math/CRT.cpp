//依存: ExtGCD.cpp
//m[i] > 0。x = r[i] (mod m[i]) の {最小非負解, lcm}。解なしなら {0, -1}
pair<ll, ll> crt(const vector<ll>& r, const vector<ll>& m) {
    assert(r.size() == m.size());
    ll R = 0, M = 1;
    for (int i = 0; i < (int)r.size(); i++) {
        ll x, y;
        ll d = extGCD(M, m[i], x, y);
        ll diff = r[i] - R;
        if (diff % d) return {0, -1};
        ll mod = m[i] / d;
        ll t = (ll)((__int128)(diff / d) * x % mod);
        if (t < 0) t += mod;
        R += M * t;
        M *= mod;
        R %= M;
    }
    return {R, M};
}

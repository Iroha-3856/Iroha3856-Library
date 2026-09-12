// 依存: ExtGCD.cpp
// m[i] > 0。x = r[i] (mod m[i]) の {最小非負解, lcm}。解なしなら {0, -1}
// 使いどころ: 周期の異なる複数の条件を一つの周期へまとめる場合。法同士は互いに素でなくてよい。
// 具体例: r={2, 6}, m={4, 8} なら {6, 8}。x=6+8k が両方の合同式を満たす。
// 使い方:
// auto [x, mod] = crt(remainders, moduli); とすると全合同式を一つへまとめる。
// mod==-1 なら矛盾。そうでなければ全解は x+k*mod で、0<=x<mod。
// 法は互いに素でなくてもよい。法の lcm と途中の積が ll に収まること。
// 合同式 x=r[i] mod m[i] を統合し、{最小非負解, lcm}、矛盾時は {0, -1} を返す。
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

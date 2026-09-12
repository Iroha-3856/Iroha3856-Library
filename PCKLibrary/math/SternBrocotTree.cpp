// 正の既約分数 p/q の Stern-Brocot 経路を run-length で返す。O(log max(p, q))
// false は L、true は R。逆変換の分子・分母は ll に収まること
// 使いどころ: 分数を L/R の順序構造で探索する場合や、分母上限つきで実数を有理近似する場合。
// 具体例: sternBrocotPath(3, 2)={{R, 1}, {L, 1}} で、sternBrocotFraction に戻すと 3/2。
// 使い方:
// auto path = sternBrocotPath(p, q); で既約正分数への L/R 移動を run-length で得る。
// sternBrocotFraction(path) は経路を分数へ戻す。path の count は正で方向は交互でなくてもよい。
// rationalApproximation(x, maxDen) は分母<=maxDen で x に最も近い非負分数を一つ返す。
// 同距離では分母が小さい方、さらに同じなら分子が小さい方を選ぶ。
// 既約正分数 p/q へ至る Stern-Brocot 木上の経路を run-length で返す。
vector<pair<bool, ll>> sternBrocotPath(ll p, ll q) {
    assert(p > 0 and q > 0 and gcd(p, q) == 1);
    vector<pair<bool, ll>> path;
    while (p != q) {
        if (p < q) {
            ll k = (q - 1) / p;
            path.push_back({false, k});
            q -= k * p;
        } else {
            ll k = (p - 1) / q;
            path.push_back({true, k});
            p -= k * q;
        }
    }
    return path;
}

// L/R の run-length 経路に対応する既約分数 {分子, 分母} を返す。
pair<ll, ll> sternBrocotFraction(const vector<pair<bool, ll>>& path) {
    ll lp = 0, lq = 1, rp = 1, rq = 0;
    for (auto [right, k] : path) {
        if (right) lp += k * rp, lq += k * rq;
        else rp += k * lp, rq += k * lq;
    }
    return {lp + rp, lq + rq};
}

// x >= 0 に最も近い、分母が maxDen 以下の非負有理数
// 非負実数 x を分母 maxDen 以下で最良近似する分数を返す。
pair<ll, ll> rationalApproximation(long double x, ll maxDen) {
    assert(x >= 0 and maxDen >= 1);
    ll p0 = 0, q0 = 1, p1 = 1, q1 = 0;
    long double y = x;
    while (true) {
        ll a = (ll)floor(y);
        if ((__int128_t)q0 + (__int128_t)a * q1 > maxDen) break;
        ll p2 = p0 + a * p1, q2 = q0 + a * q1;
        p0 = p1; q0 = q1; p1 = p2; q1 = q2;
        long double frac = y - a;
        if (frac < 1e-30L) return {p1, q1};
        y = 1 / frac;
    }
    ll k = (maxDen - q0) / q1;
    pair<ll, ll> a = {p0 + k * p1, q0 + k * q1};
    pair<ll, ll> b = {p1, q1};
    long double da = abs(x - (long double)a.first / a.second);
    long double db = abs(x - (long double)b.first / b.second);
    return da < db ? a : b;
}

//N >= 1。{l, r, q} は l <= i < r で floor(N / i) = q
//l の昇順に O(sqrt N) 個を返す。計算量 O(sqrt N)
vector<tuple<ll, ll, ll>> enumerateQuotients(ll N) {
    assert(N >= 1);
    vector<tuple<ll, ll, ll>> ret;
    for (ll l = 1; l <= N;) {
        ll q = N / l;
        ll r = N / q + 1;
        ret.push_back({l, r, q});
        l = r;
    }
    return ret;
}

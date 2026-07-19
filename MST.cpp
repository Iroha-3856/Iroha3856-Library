//MST
//{辺長, u, v} の vector<tuple<ll, int, int>> を渡す
//m = false : 最小全域木、m = true : 最大全域木
long long MST(vector<tuple<long long, int, int>> E, int N, bool m) {
    int M = (int)E.size();
    if (!m) sort(E.begin(), E.end());
    else sort(E.begin(), E.end(), greater<tuple<long long, int, int>>());
    atcoder::dsu d(N);
    long long ret = 0;
    for (int i=0; i<M; i++) {
        if (!d.same(get<1>(E[i]), get<2>(E[i]))) {
            ret+=(long long) get<0>(E[i]);
            d.merge(get<1>(E[i]), get<2>(E[i]));
        }
    }
    return ret;
}
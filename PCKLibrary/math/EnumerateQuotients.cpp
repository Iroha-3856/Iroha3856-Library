// N >= 1。{l, r, q} は l <= i < r で floor(N / i) = q
// l の昇順に O(sqrt N) 個を返す。計算量 O(sqrt N)
// 使いどころ: N/i だけで値が決まる和や DP を、同じ商を持つ添字ごとにまとめる場合。
// 具体例: N=10 の末尾二区間は {4, 6, 2}, {6, 11, 1}。4<=i<6 では floor(10/i)=2。
// 使い方:
// for (auto [l, r, q] : enumerateQuotients(N)) で、同じ N/i を持つ添字区間をまとめて走査する。
// 各区間は整数 i の 1-indexed 半開区間 [l, r)。全区間を連結すると [1, N+1) になる。
// sum_{i=1}^N f(N/i) のような式を (r-l)*f(q) として高速に集計できる。
// floor(N/i) が一定となる全区間を {l, r, 値} の昇順列として返す。
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

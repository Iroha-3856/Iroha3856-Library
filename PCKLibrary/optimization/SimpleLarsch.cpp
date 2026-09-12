// noshi 式簡易 LARSCH。dp[i] = min_{k<i}(dp[k] + cost(k, i))、cost は Monge
// ランダムアクセス版 Θ(N log N)。argmin の暫定値を利用するオンライン分割統治
// 使いどころ: dp[k] を順次確定しながら使う Monge DP で、短い実装を優先する場合。
// 具体例: initial=0, cost(k, i)=(i-k)^2 なら N=3 の dp は {0, 1, 2, 3}。
// 使い方:
// auto [dp, opt] = simpleLarsch<T>(N, initial, INF, cost); で dp[0..N] をまとめて求める。
// cost(k, i) は k<i だけで呼ばれ、dp[k] を参照してもよい。opt[i] が最適遷移元。
// 遷移行列 dp[k]+cost(k, i) の argmin 単調性を導く Monge 条件が必要。
// 実装を短くしたい場合はこちら、線形時間が必要なら Larsch.cpp の larschDP を使う。
template<class T, class Cost>
// Monge cost のオンライン DP 値と最適遷移元を O(N log N) で返す。
pair<vector<T>, vector<int>> simpleLarsch(int N, T initial, T infinity, const Cost& cost) {
    vector<T> dp(N + 1, infinity);
    vector<int> argmin(N + 1);
    dp[0] = initial;
    // 遷移元 k で dp[i] と argmin[i] を更新する。無効な下三角外は無視する。
    auto check = [&](int i, int k) {
        if (k < 0 or i <= k) return;
        T x = dp[k] + cost(k, i);
        if (x < dp[i]) dp[i] = x, argmin[i] = k;
    };
    // [l, r] の確定済み左側から右側候補を更新するオンライン分割統治。
    function<void(int, int)> solve = [&](int l, int r) {
        if (r - l == 1) return;
        int m = (l + r) / 2;
        for (int k = argmin[l]; k <= argmin[r]; k++) check(m, k);
        solve(l, m);
        for (int k = l + 1; k <= m; k++) check(r, k);
        solve(m, r);
    };
    if (N) check(N, 0), solve(0, N);
    return {dp, argmin};
}

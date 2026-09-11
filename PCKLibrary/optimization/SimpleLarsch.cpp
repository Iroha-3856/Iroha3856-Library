//noshi 式簡易 LARSCH。dp[i] = min_{k<i}(dp[k] + cost(k, i))、cost は Monge
//ランダムアクセス版 Θ(N log N)。argmin の暫定値を利用するオンライン分割統治
template<class T, class Cost>
pair<vector<T>, vector<int>> simpleLarsch(int N, T initial, T infinity, const Cost& cost) {
    vector<T> dp(N + 1, infinity);
    vector<int> argmin(N + 1);
    dp[0] = initial;
    auto check = [&](int i, int k) {
        if (k < 0 or i <= k) return;
        T x = dp[k] + cost(k, i);
        if (x < dp[i]) dp[i] = x, argmin[i] = k;
    };
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

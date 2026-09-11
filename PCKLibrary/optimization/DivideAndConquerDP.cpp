//next[j] = min_{0 <= i < j}(previous[i] + cost(i, j)) の 1 行を計算
//argmin が j に対して単調なら O(N log N)。argmin も返す
template<class T, class Cost>
pair<vector<T>, vector<int>> divideAndConquerDP(const vector<T>& previous, T infinity,
                                                const Cost& cost) {
    int N = (int)previous.size();
    vector<T> next(N, infinity);
    vector<int> argmin(N, -1);
    function<void(int, int, int, int)> dfs = [&](int l, int r, int optL, int optR) {
        if (l >= r) return;
        int m = (l + r) / 2;
        int to = min(m - 1, optR);
        for (int i = optL; i <= to; i++) {
            T x = previous[i] + cost(i, m);
            if (x < next[m]) next[m] = x, argmin[m] = i;
        }
        int opt = argmin[m] == -1 ? optL : argmin[m];
        dfs(l, m, optL, opt);
        dfs(m + 1, r, opt, optR);
    };
    if (N >= 2) dfs(1, N, 0, N - 1);
    return {next, argmin};
}

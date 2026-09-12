// next[j] = min_{0 <= i < j}(previous[i] + cost(i, j)) の 1 行を計算
// argmin が j に対して単調なら O(N log N)。argmin も返す
// 使いどころ: K 分割型 DP の一層で、最適な区切り位置 i が j とともに右へ動く場合。
// 具体例: previous={0, INF, INF, INF}, cost(i, j)=(j-i)^2 なら next={INF, 1, 4, 9}。
// 使い方:
// auto [next, opt] = divideAndConquerDP(previous, INF, cost); として DP の一層を更新する。
// cost(i, j) は 0<=i<j<N に対する区間コスト。next[0]=INF、opt[0]=-1 のまま。
// previous[i] または cost(i,j) が infinity なら到達不能として遷移候補から除外する。
// 各 j=1,...,N-1 について有限な遷移候補が少なくとも一つ存在することを仮定する。
// 最適 i が j の増加に対して非減少であることを問題側で証明してから使う。
// K 分割 DP なら previous=next を K 回繰り返し、必要なら各層の opt を保存して復元する。
template<class T, class Cost>
// argmin 単調性を使い、previous から次の DP 一層と各遷移元を返す。
pair<vector<T>, vector<int>> divideAndConquerDP(const vector<T>& previous, T infinity,
                                                const Cost& cost) {
    int N = (int)previous.size();
    vector<T> next(N, infinity);
    vector<int> argmin(N, -1);
    // 出力添字 [l, r) を、候補遷移元 [optL, optR] に制限して再帰的に解く。
    function<void(int, int, int, int)> dfs = [&](int l, int r, int optL, int optR) {
        if (l >= r) return;
        int m = (l + r) / 2;
        int to = min(m - 1, optR);
        for (int i = optL; i <= to; i++) {
            if (previous[i] == infinity) continue;
            T c = cost(i, m);
            if (c == infinity) continue;
            T x = previous[i] + c;
            if (x < next[m]) next[m] = x, argmin[m] = i;
        }
        int opt = argmin[m] == -1 ? optL : argmin[m];
        dfs(l, m, optL, opt);
        dfs(m + 1, r, opt, optR);
    };
    if (N >= 2) dfs(1, N, 0, N - 1);
    return {next, argmin};
}

// 各行の argmin が単調な H x W 行列の行最小値。O((H + W) log H)
// 同値なら左側を選ぶ。get(row, col) は必要な要素だけ計算できればよい
// 使いどころ: DP の遷移行列で、行が進むほど最適な遷移元の列が左へ戻らない場合。
// 具体例: get(i, j)=(i-j)^2 の 3x3 行列では返る argmin は {0, 1, 2}。
// 使い方:
// auto rows = monotoneMinima<T>(H, W, get); とすると rows[i]={argmin, 最小値}。
// 行列を保持せず get(i, j) ラムダで遅延評価できる。H==0 または W==0 では初期値の配列。
// 各行の最左 argmin が非減少であることを仮定する。totally monotone なら SMAWK も使える。
// SMAWK より仮定が弱く写経も追いやすい一方、比較回数は線形ではなく対数因子を持つ。
template<class T, class Get>
// 行ごとの最左 argmin が単調な行列について {argmin, 最小値} を全行分返す。
vector<pair<int, T>> monotoneMinima(int H, int W, const Get& get) {
    vector<pair<int, T>> ret(H);
    // 行区間 [top, bottom) を、許される列区間 [left, right) 内で再帰的に解く。
    function<void(int, int, int, int)> dfs = [&](int top, int bottom, int left, int right) {
        if (top >= bottom) return;
        int row = (top + bottom) / 2;
        int best = left;
        T value = get(row, left);
        for (int col = left + 1; col < right; col++) {
            T x = get(row, col);
            if (x < value) value = x, best = col;
        }
        ret[row] = {best, value};
        dfs(top, row, left, best + 1);
        dfs(row + 1, bottom, best, right);
    };
    if (H and W) dfs(0, H, 0, W);
    return ret;
}

//各行の argmin が単調な H x W 行列の行最小値。O((H + W) log H)
//同値なら左側を選ぶ。get(row, col) は必要な要素だけ計算できればよい
template<class T, class Get>
vector<pair<int, T>> monotoneMinima(int H, int W, const Get& get) {
    vector<pair<int, T>> ret(H);
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

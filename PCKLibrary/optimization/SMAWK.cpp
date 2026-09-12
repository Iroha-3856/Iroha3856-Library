// totally monotone な H x W 行列の各行 argmin。O(H + W) 回の比較
// 同値なら左側を選ぶ。H, W > 0
// 使いどころ: 行列を get で遅延評価でき、すべての部分行列でも argmin の単調性が保たれる場合。
// 具体例: get(i, j)=(i-j)^2 の 3x3 行列なら smawk(...)= {0, 1, 2}。
// 使い方:
// vector<int> argmin = smawk<T>(H, W, get); とし、最小値は get(i, argmin[i]) で得る。
// get(row, col) は行列を実体化せず値を返すラムダでよい。H, W は正。
// 行最小位置が単調なだけでは不十分で、任意の部分行列でも単調な totally monotone 性が必要。
// Monge 行列はこの条件を満たす。等値時は最左列を返す。
// 行 argmin の単調性しか証明できない場合は、条件の弱い MonotoneMinima を使う。
template<class T, class Get>
// totally monotone 行列の各行について最左 argmin 列を返す。
vector<int> smawk(int H, int W, const Get& get) {
    vector<int> answer(H, -1);
    // 指定された部分行・部分列について、列削減と奇数行再帰を行う。
    function<void(const vector<int>&, const vector<int>&)> solve =
        [&](const vector<int>& rows, const vector<int>& cols) {
        if (rows.empty()) return;
        vector<int> reduced;
        for (int col : cols) {
            while (!reduced.empty()) {
                int row = rows[(int)reduced.size() - 1];
                if (get(row, col) < get(row, reduced.back())) reduced.pop_back();
                else break;
            }
            if (reduced.size() < rows.size()) reduced.push_back(col);
        }
        vector<int> odd;
        for (int i = 1; i < (int)rows.size(); i += 2) odd.push_back(rows[i]);
        solve(odd, reduced);
        int left = 0;
        for (int i = 0; i < (int)rows.size(); i += 2) {
            if (i > 0) {
                left = (int)(find(reduced.begin(), reduced.end(), answer[rows[i - 1]]) - reduced.begin());
            }
            int right = (int)reduced.size() - 1;
            if (i + 1 < (int)rows.size()) {
                right = (int)(find(reduced.begin(), reduced.end(), answer[rows[i + 1]]) - reduced.begin());
            }
            int best = reduced[left];
            for (int j = left + 1; j <= right; j++) {
                if (get(rows[i], reduced[j]) < get(rows[i], best)) best = reduced[j];
            }
            answer[rows[i]] = best;
        }
    };
    vector<int> rows(H), cols(W);
    iota(rows.begin(), rows.end(), 0);
    iota(cols.begin(), cols.end(), 0);
    solve(rows, cols);
    return answer;
}

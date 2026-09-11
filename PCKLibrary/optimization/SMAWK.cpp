//totally monotone な H x W 行列の各行 argmin。O(H + W) 回の比較
//同値なら左側を選ぶ。H, W > 0
template<class T, class Get>
vector<int> smawk(int H, int W, const Get& get) {
    vector<int> answer(H, -1);
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

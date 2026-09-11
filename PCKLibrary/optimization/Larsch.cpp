//totally monotone な N x N 下三角行列のオンライン行 argmin。Θ(N)
//get(i, j) は j <= i でのみ呼ばれ、第 j 行の答え確定後ならその値を使ってよい
template<class T>
struct Larsch {
    struct ReduceColumn;
    struct ReduceRow {
        int N, row = 0, state = 0;
        function<T(int, int)> get;
        unique_ptr<ReduceColumn> child;

        ReduceRow(int n) : N(n) {
            if (N / 2) child = make_unique<ReduceColumn>(N / 2);
        }
        void setFunction(function<T(int, int)> f) {
            get = f;
            if (child) child->setFunction([&](int i, int j) { return get(2 * i + 1, j); });
        }
        int next() {
            int i = row++;
            if (i % 2 == 0) {
                int left = state;
                int right = i + 1 == N ? N - 1 : child->next();
                state = right;
                int ret = left;
                for (int j = left + 1; j <= right; j++) if (get(i, j) < get(i, ret)) ret = j;
                return ret;
            }
            return get(i, state) <= get(i, i) ? state : i;
        }
    };
    struct ReduceColumn {
        int N, row = 0;
        vector<int> cols;
        function<T(int, int)> get;
        ReduceRow child;

        ReduceColumn(int n) : N(n), child(n) {}
        void setFunction(function<T(int, int)> f) {
            get = f;
            child.setFunction([&](int i, int j) { return get(i, cols[j]); });
        }
        int next() {
            int i = row++;
            vector<int> candidate = i == 0 ? vector<int>{0} : vector<int>{2 * i - 1, 2 * i};
            for (int col : candidate) {
                while ((int)cols.size() != i
                       and get((int)cols.size() - 1, cols.back()) > get((int)cols.size() - 1, col)) {
                    cols.pop_back();
                }
                if ((int)cols.size() != N) cols.push_back(col);
            }
            return cols[child.next()];
        }
    };
    unique_ptr<ReduceRow> root;

    Larsch(int N, function<T(int, int)> get) : root(make_unique<ReduceRow>(N)) {
        root->setFunction(get);
    }
    int next() { return root->next(); }
};

//dp[i+1] を確定した直後から、その値を cost の計算に使える
template<class T, class Cost>
vector<T> larschDP(int N, T initial, const Cost& cost) {
    vector<T> dp(N + 1);
    dp[0] = initial;
    Larsch<T> solver(N, [&](int i, int j) { return dp[j] + cost(j, i + 1); });
    for (int i = 0; i < N; i++) {
        int j = solver.next();
        dp[i + 1] = dp[j] + cost(j, i + 1);
    }
    return dp;
}

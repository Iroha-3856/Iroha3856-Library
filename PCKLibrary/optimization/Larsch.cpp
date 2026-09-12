// totally monotone な N x N 下三角行列のオンライン行 argmin。Θ(N)
// get(i, j) は j <= i でのみ呼ばれ、第 j 行の答え確定後ならその値を使ってよい
// 使いどころ: dp[j] が確定して初めて列 j を評価できる Monge DP を、オンラインのまま線形時間で解く場合。
// 具体例: initial=0, cost(j, i)=(i-j)^2 なら larschDP(3, ...)= {0, 1, 2, 3}。
// 使い方:
// Larsch<T> solver(N, get); を作り、行 i=0..N-1 の順に int col=solver.next(); を一度ずつ呼ぶ。
// col は行 i の最小列。次の next より前に、この結果から定まるオンライン状態を更新してよい。
// 通常の dp[i]=min_{j<i}(dp[j]+cost(j, i)) には larschDP(N, initial, cost) を使う。
// 下三角の評価行列が totally monotone、DP 版では対応する cost が Monge であることを要する。
// 実装量を優先するなら O(N log N) の SimpleLarsch、線形時間が必要ならこちらを選ぶ。
template<class T>
struct Larsch {
    struct ReduceColumn;
    struct ReduceRow {
        int N, row = 0, state = 0;
        function<T(int, int)> get;
        unique_ptr<ReduceColumn> child;

        // n 行の行削減器を作り、必要なら半分サイズの列削減器を再帰的に持つ。
        ReduceRow(int n) : N(n) {
            if (N / 2) child = make_unique<ReduceColumn>(N / 2);
        }
        // この層の行列要素取得関数を設定し、子が見る奇数行へ写像する。
        void setFunction(function<T(int, int)> f) {
            get = f;
            // 子の行 i を元行列の奇数行 2*i+1 へ対応させる。
            if (child) child->setFunction([&](int i, int j) { return get(2 * i + 1, j); });
        }
        // 次の行の argmin 列を返し、内部の処理行を一つ進める。
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

        // n 列の列削減器と、削減後行列を扱う子の行削減器を作る。
        ReduceColumn(int n) : N(n), child(n) {}
        // 元行列の取得関数を設定し、cols を介した削減後関数を子へ渡す。
        void setFunction(function<T(int, int)> f) {
            get = f;
            // 子の列 j を削減後に残った元列 cols[j] へ対応させる。
            child.setFunction([&](int i, int j) { return get(i, cols[j]); });
        }
        // 次の候補列を stack で削減し、子が返した位置を元の列番号へ戻す。
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

    // N 行の下三角 totally monotone 行列と遅延評価関数 get を受け取る。
    Larsch(int N, function<T(int, int)> get) : root(make_unique<ReduceRow>(N)) {
        root->setFunction(get);
    }
    // 未処理の次行について最左 argmin 列を返す。ちょうど N 回呼べる。
    int next() { return root->next(); }
};

// dp[i+1] を確定した直後から、その値を cost の計算に使える
template<class T, class Cost>
// Monge cost のオンライン遷移 dp[i]=min_{j<i}(dp[j]+cost(j, i)) を線形時間で返す。
vector<T> larschDP(int N, T initial, const Cost& cost) {
    vector<T> dp(N + 1);
    dp[0] = initial;
    // 行 i・列 j を遷移 dp[j]+cost(j, i+1) として遅延評価する。
    Larsch<T> solver(N, [&](int i, int j) { return dp[j] + cost(j, i + 1); });
    for (int i = 0; i < N; i++) {
        int j = solver.next();
        dp[i + 1] = dp[j] + cost(j, i + 1);
    }
    return dp;
}

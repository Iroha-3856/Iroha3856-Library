template<class T>
struct CumulativeSum2D {
    bool done;
    int H, W;
    vector<vector<T>> S;
    CumulativeSum2D() : CumulativeSum2D(0, 0) {}
    CumulativeSum2D(int h, int w) : CumulativeSum2D(vector<vector<T>>(h, vector<T>(w))) {}
    CumulativeSum2D(vector<vector<T>> A) {
        done = false;
        H = (int)A.size();
        W = (int)A[0].size();
        S.resize(H+1, vector<T>(W+1, 0));
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) S[i+1][j+1] = A[i][j];
        }
    }
    //累積
    void build() {
        assert(!done);
        for (int i = 1; i <= H; i++) {
            for (int j = 1; j <= W; j++) {
                S[i][j] += S[i][j-1];
            }
        }
        for (int j = 1; j <= W; j++) {
            for (int i = 1; i <= H; i++) S[i][j] += S[i-1][j];
        }
        done = true;
    }
    //加算（累積前のみ）
    T add(int r, int c, T x) {
        assert(!done);
        assert(0 <= r and r < H and 0 <= c and c < W);
        return S[r+1][c+1] += x;
    }
    //代入（累積前のみ）
    T set(int r, int c, T x) {
        assert(!done);
        return S[r+1][c+1] = x;
    }
    //取得（累積前、累積後両方とも可能だが、どちらを想定するかをexpected_doneで渡す）
    [[nodiscard]]
    T get(int r, int c, bool expect_done) {
        assert(expect_done == done);
        return S[r+1][c+1];
    }
    //区間和取得（累積後のみ）
    //行、列とも半開区間で与える
    [[nodiscard]]
    T sum(int left_row, int right_row, int left_column, int right_column) {
        assert(done);
        return + S[right_row][right_column] 
               - S[right_row][left_column] 
               - S[left_row][right_column] 
               + S[left_row][left_column];
    }
};
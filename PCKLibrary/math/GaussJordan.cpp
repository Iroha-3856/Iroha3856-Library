// Gauss-Jordan 消去。consistent=false は解なし
// particular は特解、basis は斉次解空間の基底。計算量 O(H W min(H, W))
// 使いどころ: 実数・有限体・GF(2) 上の連立一次方程式で、解の存在、一例、自由度をまとめて得る場合。
// 具体例: x+y=3, x-y=1 なら particular={2, 1}、basis は空で一意解になる。
// 使い方:
// A x = b に対し、modint 等の体なら linearEquationField(A, b)、実数なら linearEquationReal(A, b)。
// GF(2) は各行を bitset<MAX> に詰めて linearEquationGF2<MAX>(A, b, 変数数) を呼ぶ。
// consistent を確認後、全解は particular + basis の任意線形結合。basis.empty() なら一意解。
// 整数型を Field 版へ渡さない。実数版の第3引数で零判定 eps を調整できる。
template<class T>
struct LinearSolution {
    bool consistent;
    int rank;
    vector<T> particular;
    vector<vector<T>> basis;
};

template<class T>
// 体 T 上で A*x=b を解き、特解・斉次解基底・rank を返す。
LinearSolution<T> linearEquationField(vector<vector<T>> A, vector<T> b) {
    int H = (int)A.size(), W = H ? (int)A[0].size() : 0;
    vector<int> pivot;
    int rank = 0;
    for (int col = 0; col < W and rank < H; col++) {
        int row = rank;
        while (row < H and A[row][col] == T(0)) row++;
        if (row == H) continue;
        swap(A[rank], A[row]); swap(b[rank], b[row]);
        T inv = T(1) / A[rank][col];
        for (int j = col; j < W; j++) A[rank][j] *= inv;
        b[rank] *= inv;
        for (int i = 0; i < H; i++) if (i != rank and A[i][col] != T(0)) {
            T x = A[i][col];
            for (int j = col; j < W; j++) A[i][j] -= x * A[rank][j];
            b[i] -= x * b[rank];
        }
        pivot.push_back(col);
        rank++;
    }
    for (int i = rank; i < H; i++) if (b[i] != T(0)) return {false, rank, {}, {}};
    vector<T> solution(W, T(0));
    for (int i = 0; i < rank; i++) solution[pivot[i]] = b[i];
    vector<bool> isPivot(W);
    for (int x : pivot) isPivot[x] = true;
    vector<vector<T>> basis;
    for (int free = 0; free < W; free++) if (!isPivot[free]) {
        vector<T> x(W, T(0));
        x[free] = T(1);
        for (int i = 0; i < rank; i++) x[pivot[i]] = T(0) - A[i][free];
        basis.push_back(x);
    }
    return {true, rank, solution, basis};
}

// 実数上で部分 pivot 選択と eps 零判定を使って A*x=b を解く。
LinearSolution<long double> linearEquationReal(vector<vector<long double>> A,
                                                vector<long double> b,
                                                long double eps = 1e-12L) {
    int H = (int)A.size(), W = H ? (int)A[0].size() : 0;
    vector<int> pivot;
    int rank = 0;
    for (int col = 0; col < W and rank < H; col++) {
        int row = rank;
        for (int i = rank; i < H; i++) if (abs(A[row][col]) < abs(A[i][col])) row = i;
        if (abs(A[row][col]) <= eps) continue;
        swap(A[rank], A[row]); swap(b[rank], b[row]);
        long double inv = 1 / A[rank][col];
        for (int j = col; j < W; j++) A[rank][j] *= inv;
        b[rank] *= inv;
        for (int i = 0; i < H; i++) if (i != rank and abs(A[i][col]) > eps) {
            long double x = A[i][col];
            for (int j = col; j < W; j++) A[i][j] -= x * A[rank][j];
            b[i] -= x * b[rank];
        }
        pivot.push_back(col);
        rank++;
    }
    for (int i = rank; i < H; i++) if (abs(b[i]) > eps) return {false, rank, {}, {}};
    vector<long double> solution(W);
    for (int i = 0; i < rank; i++) solution[pivot[i]] = b[i];
    vector<bool> isPivot(W);
    for (int x : pivot) isPivot[x] = true;
    vector<vector<long double>> basis;
    for (int free = 0; free < W; free++) if (!isPivot[free]) {
        vector<long double> x(W);
        x[free] = 1;
        for (int i = 0; i < rank; i++) x[pivot[i]] = -A[i][free];
        basis.push_back(x);
    }
    return {true, rank, solution, basis};
}

// A は各式の係数 bitset、b は右辺。変数数 W <= MAX
template<int MAX>
// GF(2) 上の係数行列を bitset 行として受け、A*x=b の解空間を返す。
LinearSolution<int> linearEquationGF2(vector<bitset<MAX>> A, vector<int> b, int W) {
    int H = (int)A.size(), rank = 0;
    vector<int> pivot;
    for (int col = 0; col < W and rank < H; col++) {
        int row = rank;
        while (row < H and !A[row][col]) row++;
        if (row == H) continue;
        swap(A[rank], A[row]); swap(b[rank], b[row]);
        for (int i = 0; i < H; i++) if (i != rank and A[i][col]) {
            A[i] ^= A[rank]; b[i] ^= b[rank];
        }
        pivot.push_back(col);
        rank++;
    }
    for (int i = rank; i < H; i++) if (b[i]) return {false, rank, {}, {}};
    vector<int> solution(W), isPivot(W);
    for (int i = 0; i < rank; i++) solution[pivot[i]] = b[i], isPivot[pivot[i]] = true;
    vector<vector<int>> basis;
    for (int free = 0; free < W; free++) if (!isPivot[free]) {
        vector<int> x(W);
        x[free] = 1;
        for (int i = 0; i < rank; i++) x[pivot[i]] = A[i][free];
        basis.push_back(x);
    }
    return {true, rank, solution, basis};
}

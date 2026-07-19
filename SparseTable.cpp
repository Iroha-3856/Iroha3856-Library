template<class T>
struct SparseTable {
    int n;
    vector<vector<T>> data;

    //x = 0は未定義
    int msb(int x) {
        return 31 - __builtin_clz(x);
    }

    SparseTable(vector<T> A) : n((int)A.size()) {
        int logn = 1;
        while((2<<logn) <= n) logn++;
        data.resize(logn+1, vector<T>(n));
        data[0] = A;
        for (int i = 1; i <= logn; i++) {
            for (int j = 0; j + (1<<i) <= n; j++) {
                data[i][j] = min(data[i-1][j], data[i-1][j+(1<<(i-1))]);
            }
        }
    }

    T query(int l, int r, const T INF) {
        assert(0 <= l and l < r and r <= n);
        if (l == r) return INF;
        int b = msb(r - l);
        return min(data[b][l], data[b][r - (1<<b)]);
    }
};
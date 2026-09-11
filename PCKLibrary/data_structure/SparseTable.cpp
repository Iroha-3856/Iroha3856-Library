//op は min, max, gcd など冪等な演算。構築 O(NlogN)、query O(1)
template<class T, T(*op)(T, T)>
struct SparseTable {
    vector<vector<T>> data;

    SparseTable() {}
    SparseTable(const vector<T>& A) {
        build(A);
    }
    void build(const vector<T>& A) {
        int N = (int)A.size(), log = 1;
        while ((1 << log) <= N) log++;
        data.assign(log, vector<T>(N));
        data[0] = A;
        for (int k = 1; k < log; k++) {
            for (int i = 0; i + (1 << k) <= N; i++) {
                data[k][i] = op(data[k - 1][i], data[k - 1][i + (1 << (k - 1))]);
            }
        }
    }
    //半開区間 [l, r)、空区間は不可
    T query(int l, int r) const {
        int k = 31 - __builtin_clz(r - l);
        return op(data[k][l], data[k][r - (1 << k)]);
    }
};

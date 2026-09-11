//静的列の区間積。結合則のみを仮定し、構築 O(N log N)、prod O(1)
//0-indexed、半開区間 [l, r)。空区間は e() を返す
template<class S, S(*op)(S, S), S(*e)()>
struct DisjointSparseTable {
    int N, log;
    vector<S> A;
    vector<vector<S>> table;

    DisjointSparseTable(const vector<S>& a) : N((int)a.size()), A(a) {
        log = 1;
        while ((1 << log) < max(1, N)) log++;
        table.assign(log, vector<S>(N));
        for (int k = 0; k < log; k++) {
            int len = 1 << k;
            for (int m = len; m < N; m += 2 * len) {
                table[k][m - 1] = A[m - 1];
                for (int i = m - 2; i >= m - len; i--) {
                    table[k][i] = op(A[i], table[k][i + 1]);
                }
                table[k][m] = A[m];
                for (int i = m + 1; i < min(N, m + len); i++) {
                    table[k][i] = op(table[k][i - 1], A[i]);
                }
            }
        }
    }
    S prod(int l, int r) const {
        assert(0 <= l and l <= r and r <= N);
        if (l == r) return e();
        if (l + 1 == r) return A[l];
        int k = 31 - __builtin_clz(l ^ (r - 1));
        return op(table[k][l], table[k][r - 1]);
    }
};

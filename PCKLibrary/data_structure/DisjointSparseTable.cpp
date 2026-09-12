// 静的列の区間積。結合則のみを仮定し、構築 O(N log N)、prod O(1)
// 0-indexed、半開区間 [l, r)。空区間は e() を返す
// 使い方:
// S op(S left, S right), S e() を用意して DisjointSparseTable<S, op, e> table(A);
// table.prod(l, r) で A[l] op ... op A[r - 1] を得る。
// 更新はできない。min に限らず、行列積や文字列結合のような非可換演算にも使える。
// 使いどころ: 配列が不変で、和・行列積など通常の Sparse Table では扱えない区間積を多数求める場合。
// 具体例: A={"a", "b", "c"}、op=文字列結合なら prod(1, 3) は "bc"。
// S は要素型、op は左から右へ結合する関数、e は空区間を表す単位元である。
template<class S, S(*op)(S, S), S(*e)()>
struct DisjointSparseTable {
    int N, log;
    vector<S> A;
    vector<vector<S>> table;

    // 静的配列 a から左右の部分積を前計算する。空配列も構築できる。
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
    // 半開区間 [l, r) を左から順に畳み込む。空区間なら e() を返す。
    S prod(int l, int r) const {
        assert(0 <= l and l <= r and r <= N);
        if (l == r) return e();
        if (l + 1 == r) return A[l];
        int k = 31 - __builtin_clz(l ^ (r - 1));
        return op(table[k][l], table[k][r - 1]);
    }
};

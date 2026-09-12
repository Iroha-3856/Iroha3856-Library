// op は min, max, gcd など冪等な演算。構築 O(NlogN)、query O(1)
// 使い方:
// 冪等かつ結合的な op を用意し SparseTable<T, op> table(A);
// table.query(l, r) で非空半開区間 [l, r) の値を得る。更新はできない。
// 和や積のように重複適用できない演算には Disjoint Sparse Table を使う。
// 使いどころ: 変更されない配列への min/max/gcd 問い合わせを O(1) にしたい場合。
// 具体例: A={5, 2, 7, 3}、op=min なら query(1, 4)=2。
// 冪等とは op(x, x)=x のこと。この性質で重なる二区間を使うため、加算には使用できない。
template<class T, T(*op)(T, T)>
struct SparseTable {
    vector<vector<T>> data;

    // 未構築の表を作る。後から build(A) を一度呼ぶ。
    SparseTable() {}
    // 配列 A から冪等演算の表を構築する。
    SparseTable(const vector<T>& A) {
        build(A);
    }
    // 現在の表を捨て、配列 A で再構築する。A は空でもよい。
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
    // 非空半開区間 [l, r) の積を返す。0 <= l < r <= N が必要。
    T query(int l, int r) const {
        int k = 31 - __builtin_clz(r - l);
        return op(data[k][l], data[k][r - (1 << k)]);
    }
};

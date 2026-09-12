// 依存: graph/StronglyConnectedComponents.cpp
// 2-SAT。addClause(i, f, j, g) は (xi == f) or (xj == g)。O(N + M)
// 使い方:
// TwoSAT sat(N); sat.addClause(i, true, j, false); のように各 OR 条件を追加する。
// auto answer = sat.solve(); が nullopt なら充足不能、そうでなければ (*answer)[i] が xi。
// addImplication(i, f, j, g) は (xi==f) => (xj==g)。単独リテラルは addClause(i,f,i,f)。
// 使いどころ: 各項目を true/false のどちらかに決め、禁止される二値の組を OR 条件で表せる場合。
// 具体例: x0 または x1 を真にする条件は addClause(0, true, 1, true)。
// 「x0 と x1 を同時に真にしない」は (!x0 OR !x1) なので addClause(0, false, 1, false)。
struct TwoSAT {
    int N;
    vector<vector<int>> G;
    vector<bool> answer;

    // bool 変数 x0, ..., x(n-1) を持つ空の 2-SAT を作る。
    TwoSAT(int n) : N(n), G(2 * n) {}
    // リテラル (xv==value) に対応する含意グラフの頂点番号を返す。
    int id(int v, bool value) const { return 2 * v + value; }
    // 含意 (xi==f) => (xj==g) と、その対偶 (xj!=g) => (xi!=f) を追加する。
    void addImplication(int i, bool f, int j, bool g) {
        G[id(i, f)].push_back(id(j, g));
        G[id(j, !g)].push_back(id(i, !f));
    }
    // 節 (xi==f) OR (xj==g) を含意グラフへ追加する。
    void addClause(int i, bool f, int j, bool g) {
        addImplication(i, !f, j, g);
    }
    // 充足可能なら変数値を返し、不可能なら nullopt を返す。
    optional<vector<bool>> solve() {
        StronglyConnectedComponents scc(G);
        answer.resize(N);
        for (int i = 0; i < N; i++) {
            if (scc.component[id(i, false)] == scc.component[id(i, true)]) return nullopt;
            answer[i] = scc.component[id(i, false)] < scc.component[id(i, true)];
        }
        return answer;
    }
};

//依存: graph/StronglyConnectedComponents.cpp
//2-SAT。addClause(i, f, j, g) は (xi == f) or (xj == g)。O(N + M)
struct TwoSAT {
    int N;
    vector<vector<int>> G;
    vector<bool> answer;

    TwoSAT(int n) : N(n), G(2 * n) {}
    int id(int v, bool value) const { return 2 * v + value; }
    void addImplication(int i, bool f, int j, bool g) {
        G[id(i, f)].push_back(id(j, g));
    }
    void addClause(int i, bool f, int j, bool g) {
        addImplication(i, !f, j, g);
        addImplication(j, !g, i, f);
    }
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

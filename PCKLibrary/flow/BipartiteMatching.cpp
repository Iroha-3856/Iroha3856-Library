//二部グラフ最大マッチング。O(VE)
struct BipartiteMatching {
    vector<vector<int>> G;
    vector<int> match;
    vector<bool> used;

    BipartiteMatching(int L, int R) : G(L), match(R, -1), used(L) {}
    void addEdge(int left, int right) {
        G[left].push_back(right);
    }
    bool dfs(int v) {
        if (used[v]) return false;
        used[v] = true;
        for (int to : G[v]) {
            if (match[to] == -1 or dfs(match[to])) {
                match[to] = v;
                return true;
            }
        }
        return false;
    }
    int solve() {
        int ret = 0;
        fill(match.begin(), match.end(), -1);
        for (int v = 0; v < (int)G.size(); v++) {
            fill(used.begin(), used.end(), false);
            ret += dfs(v);
        }
        return ret;
    }
    //{左側, 右側} の組を返す。solve() 後に呼ぶ
    vector<pair<int, int>> getMatching() const {
        vector<pair<int, int>> ret;
        for (int right = 0; right < (int)match.size(); right++) {
            if (match[right] != -1) ret.push_back({match[right], right});
        }
        return ret;
    }
};

// 二部グラフ最大マッチング。O(VE)
// 使い方:
// BipartiteMatching matching(L, R); matching.addEdge(left, right); で辺を追加する。
// int size = matching.solve(); の後、getMatching() で {左頂点, 右頂点} の組を得る。
// solve 後の match[r] は右頂点 r に対応する左頂点、未マッチなら -1。
// 使いどころ: 左右二群の一対一対応を最大数作る問題。一般グラフの matching には使えない。
// 具体例: 左0から右1、左1から右0へ辺があれば solve()=2、getMatching() は二組を返す。
// solve は現在の matching を一度消して計算し直す。辺を追加した後に再実行することはできる。
struct BipartiteMatching {
    vector<vector<int>> G;
    vector<int> match;
    vector<bool> used;

    // 左 L 頂点、右 R 頂点からなる空の二部グラフを作る。
    BipartiteMatching(int L, int R) : G(L), match(R, -1), used(L) {}
    // 左頂点 left と右頂点 right の間に辺を追加する。
    void addEdge(int left, int right) {
        G[left].push_back(right);
    }
    // 左頂点 v から増加路を DFS し、見つかれば matching を更新して true を返す。
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
    // Kuhn 法で最大マッチングを構築し、その辺数を返す。
    int solve() {
        int ret = 0;
        fill(match.begin(), match.end(), -1);
        for (int v = 0; v < (int)G.size(); v++) {
            fill(used.begin(), used.end(), false);
            ret += dfs(v);
        }
        return ret;
    }
    // 現在の match 配列を {左頂点, 右頂点} の組へ変換する。solve() 後に呼ぶ。
    vector<pair<int, int>> getMatching() const {
        vector<pair<int, int>> ret;
        for (int right = 0; right < (int)match.size(); right++) {
            if (match[right] != -1) ret.push_back({match[right], right});
        }
        return ret;
    }
};

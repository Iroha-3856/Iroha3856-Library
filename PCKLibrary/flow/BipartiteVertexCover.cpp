// 二部最大マッチング後の最小頂点被覆・最大独立集合。O(V + E)
// rightMatch[r] は右 r と対応する左頂点、未マッチなら -1
// 使い方:
// BipartiteMatching::solve() 後の隣接リスト G と match を bipartiteVertexCover(G, match) へ渡す。
// leftCover/rightCover の合併が最小頂点被覆、leftIndependent/rightIndependent がその補集合。
// rightMatch は最大マッチングでなければ König の定理による最小性が保証されない。
// 使いどころ: 全辺を覆う最小頂点集合、または辺で結ばれない最大頂点集合を二部グラフで求める場合。
// 具体例: 左0--右0 の一辺だけなら最小頂点被覆の大きさは1、最大独立集合の大きさも1。
// leftCover と rightCover を合わせた個数は最大 matching サイズと等しい。
// 最大独立集合はグラフ全頂点から最小頂点被覆を除いた集合で、左右それぞれに分けて返す。
struct BipartiteCoverResult {
    vector<int> leftCover, rightCover, leftIndependent, rightIndependent;
};

// 最大 matching の交互路から最小頂点被覆と、その補集合である最大独立集合を返す。
BipartiteCoverResult bipartiteVertexCover(const vector<vector<int>>& G,
                                           const vector<int>& rightMatch) {
    int L = (int)G.size(), R = (int)rightMatch.size();
    vector<int> leftMatch(L, -1);
    for (int r = 0; r < R; r++) if (rightMatch[r] != -1) leftMatch[rightMatch[r]] = r;
    vector<bool> leftUsed(L), rightUsed(R);
    queue<int> Q;
    for (int l = 0; l < L; l++) if (leftMatch[l] == -1) leftUsed[l] = true, Q.push(l);
    while (!Q.empty()) {
        int l = Q.front(); Q.pop();
        for (int r : G[l]) if (leftMatch[l] != r and !rightUsed[r]) {
            rightUsed[r] = true;
            int to = rightMatch[r];
            if (to != -1 and !leftUsed[to]) leftUsed[to] = true, Q.push(to);
        }
    }
    BipartiteCoverResult ret;
    for (int l = 0; l < L; l++) {
        if (!leftUsed[l]) ret.leftCover.push_back(l);
        else ret.leftIndependent.push_back(l);
    }
    for (int r = 0; r < R; r++) {
        if (rightUsed[r]) ret.rightCover.push_back(r);
        else ret.rightIndependent.push_back(r);
    }
    return ret;
}

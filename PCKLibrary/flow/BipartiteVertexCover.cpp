//二部最大マッチング後の最小頂点被覆・最大独立集合。O(V + E)
//rightMatch[r] は右 r と対応する左頂点、未マッチなら -1
struct BipartiteCoverResult {
    vector<int> leftCover, rightCover, leftIndependent, rightIndependent;
};

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

// Suffix Array と LCP Array。doubling 法、O(N log^2 N)
// suffixArray は空 suffix を含めない。lcp[i] = LCP(sa[i], sa[i+1])
// 使いどころ: suffix の辞書順、部分文字列検索、異なる部分文字列数、suffix 間 LCP を求める場合。
// 具体例: suffixArray("banana")={5, 3, 1, 0, 4, 2}。先頭は "a"、次は "ana" である。
// 使い方:
// vector<int> sa = suffixArray(s); で suffix s[sa[i], N) の辞書順を得る。
// vector<int> lcp = lcpArray(s, sa); は長さ max(0, N-1) で、隣接 suffix の LCP を返す。
// パターン検索は sa 上を lower_bound/upper_bound、任意二 suffix の LCP は lcp 上の RMQ で行う。
// 非空 suffix の開始位置を suffix の辞書順に返す。
vector<int> suffixArray(const string& s) {
    int N = (int)s.size();
    vector<int> sa(N), rank(N), next(N);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < N; i++) rank[i] = (unsigned char)s[i];
    for (int len = 1; len < N; len <<= 1) {
        // suffix i の比較 key を先頭 len 文字と続く len 文字の rank で表す。
        auto key = [&](int i) { return pair<int, int>{rank[i], i + len < N ? rank[i + len] : -1}; };
        // doubling key の辞書順に suffix 開始位置を並べる。
        sort(sa.begin(), sa.end(), [&](int i, int j) { return key(i) < key(j); });
        next[sa[0]] = 0;
        for (int i = 1; i < N; i++) next[sa[i]] = next[sa[i - 1]] + (key(sa[i - 1]) < key(sa[i]));
        rank.swap(next);
        if (rank[sa.back()] == N - 1) break;
    }
    return sa;
}

// 隣接する sa[i], sa[i+1] の suffix 間 LCP 長を返す。
vector<int> lcpArray(const string& s, const vector<int>& sa) {
    int N = (int)s.size();
    assert((int)sa.size() == N);
    vector<int> rank(N), lcp(max(0, N - 1));
    for (int i = 0; i < N; i++) rank[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < N; i++) {
        if (rank[i] == N - 1) continue;
        int j = sa[rank[i] + 1];
        while (i + h < N and j + h < N and s[i + h] == s[j + h]) h++;
        lcp[rank[i]] = h;
        if (h) h--;
    }
    return lcp;
}

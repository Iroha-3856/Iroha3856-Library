//Suffix Array と LCP Array。doubling 法、O(N log^2 N)
//suffixArray は空 suffix を含めない。lcp[i] = LCP(sa[i], sa[i+1])
vector<int> suffixArray(const string& s) {
    int N = (int)s.size();
    vector<int> sa(N), rank(N), next(N);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < N; i++) rank[i] = (unsigned char)s[i];
    for (int len = 1; len < N; len <<= 1) {
        auto key = [&](int i) { return pair<int, int>{rank[i], i + len < N ? rank[i + len] : -1}; };
        sort(sa.begin(), sa.end(), [&](int i, int j) { return key(i) < key(j); });
        next[sa[0]] = 0;
        for (int i = 1; i < N; i++) next[sa[i]] = next[sa[i - 1]] + (key(sa[i - 1]) < key(sa[i]));
        rank.swap(next);
        if (rank[sa.back()] == N - 1) break;
    }
    return sa;
}

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

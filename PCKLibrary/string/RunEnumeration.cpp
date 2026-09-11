//依存: string/SuffixArray.cpp
//極大周期列 run を {l, r, 最小周期} として列挙。0-indexed [l, r)、O(N log N)
struct LongestCommonExtension {
    int N, log;
    vector<int> rank;
    vector<vector<int>> table;

    LongestCommonExtension(const string& s) : N((int)s.size()) {
        vector<int> sa = suffixArray(s), lcp = lcpArray(s, sa);
        rank.resize(N);
        for (int i = 0; i < N; i++) rank[sa[i]] = i;
        log = 1;
        while ((1 << log) <= max(1, N)) log++;
        table.assign(log, vector<int>(max(0, N - 1)));
        if (N >= 2) table[0] = lcp;
        for (int k = 1; k < log; k++) {
            for (int i = 0; i + (1 << k) <= N - 1; i++) {
                table[k][i] = min(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
            }
        }
    }
    int lcp(int i, int j) const {
        if (i == j) return N - i;
        int l = rank[i], r = rank[j];
        if (l > r) swap(l, r);
        int k = 31 - __builtin_clz(r - l);
        return min(table[k][l], table[k][r - (1 << k)]);
    }
};

vector<tuple<int, int, int>> enumerateRuns(const string& s) {
    int N = (int)s.size();
    if (N == 0) return {};
    string rev = s;
    reverse(rev.begin(), rev.end());
    LongestCommonExtension right(s), left(rev);
    set<tuple<int, int, int>> runs;
    for (int reversedOrder = 0; reversedOrder < 2; reversedOrder++) {
        string t = s;
        if (reversedOrder) for (char& c : t) c = char(255 - (unsigned char)c);
        vector<int> sa = suffixArray(t), rank(N), lyndon(N, 1);
        for (int i = 0; i < N; i++) rank[sa[i]] = i;
        for (int i = N - 2; i >= 0; i--) {
            int j = i + 1;
            while (j < N and rank[i] < rank[j]) j += lyndon[j];
            lyndon[i] = j - i;
        }
        for (int i = 0; i < N; i++) {
            int p = lyndon[i], j = i + p;
            if (j >= N) continue;
            int a = i == 0 ? 0 : min(i, left.lcp(N - i, N - j));
            int b = right.lcp(i, j);
            if (a + b >= p) runs.insert({i - a, j + b, p});
        }
    }
    return vector<tuple<int, int, int>>(runs.begin(), runs.end());
}

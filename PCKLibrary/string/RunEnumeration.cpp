// 依存: string/SuffixArray.cpp
// 極大周期列 run を {l, r, 最小周期} として列挙。0-indexed [l, r)、O(N log N)
// 使いどころ: 文字列中の反復を、開始位置ごとではなく極大な周期区間として重複なく扱う場合。
// 具体例: s="ababab" には run {0, 6, 2} がある。周期2で左右へ延長できない区間 [0, 6) である。
// 使い方:
// auto runs = enumerateRuns(s); とすると各 [l, r) が周期 p を持ち、長さは 2p 以上。
// run は左右へ同じ周期で延長できない極大区間で、同一区間でも最小周期だけを返す。
// 結果は tuple の辞書順に整列・重複除去済み。周期文字列の列挙や反復構造の集計に使う。
struct LongestCommonExtension {
    int N, log;
    vector<int> rank;
    vector<vector<int>> table;

    // s の Suffix Array と LCP RMQ を構築する。
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
    // suffix s[i, N) と s[j, N) の最長共通接頭辞長を返す。
    int lcp(int i, int j) const {
        if (i == j) return N - i;
        int l = rank[i], r = rank[j];
        if (l > r) swap(l, r);
        int k = 31 - __builtin_clz(r - l);
        return min(table[k][l], table[k][r - (1 << k)]);
    }
};

// s の全 run を {左端, 右端, 最小周期} として辞書順で返す。
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

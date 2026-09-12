// prefix[i] = S[0, i + 1) の真の border の最大長。O(N)
// 使いどころ: 一つの pattern の完全一致検索、文字列の border、最小周期を決定的に求める場合。
// 具体例: kmpSearch("ababa", "aba")={0, 2}。重なっている二つの一致も返す。
// 使い方:
// prefixFunction(S) は各接頭辞の failure link 長を返す。
// kmpSearch(text, pattern) は完全一致の開始位置を昇順で返し、重なる一致も数える。
// minimumPeriod(S) は S が同じブロックの反復なら最小ブロック長、そうでなければ |S|。
// string に限らず size() と [] を持ち要素比較できる vector 等にも使える。
template<class T>
// 各接頭辞 S[0, i] の最長真 border 長を返す。
vector<int> prefixFunction(const T& S) {
    int n = (int)S.size();
    vector<int> prefix(n);
    for (int i = 1; i < n; i++) {
        int j = prefix[i - 1];
        while (j > 0 and S[i] != S[j]) j = prefix[j - 1];
        if (S[i] == S[j]) j++;
        prefix[i] = j;
    }
    return prefix;
}

// text 内で pattern が始まる位置をすべて返す。空 pattern は全境界に一致
template<class T>
// text 内で pattern と一致する開始位置を、重なる一致も含めて昇順に返す。
vector<int> kmpSearch(const T& text, const T& pattern) {
    int n = (int)text.size(), m = (int)pattern.size();
    vector<int> ret;
    if (m == 0) {
        for (int i = 0; i <= n; i++) ret.push_back(i);
        return ret;
    }
    vector<int> prefix = prefixFunction(pattern);
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 and text[i] != pattern[j]) j = prefix[j - 1];
        if (text[i] == pattern[j]) j++;
        if (j == m) {
            ret.push_back(i - m + 1);
            j = prefix[j - 1];
        }
    }
    return ret;
}

// S の最小周期。S が周期の整数回の繰り返しでなければ |S|
template<class T>
// S 全体を同じ block の反復で表せる最小 block 長を返す。
int minimumPeriod(const T& S) {
    if (S.empty()) return 0;
    vector<int> prefix = prefixFunction(S);
    int period = (int)S.size() - prefix.back();
    if ((int)S.size() % period == 0) return period;
    return (int)S.size();
}

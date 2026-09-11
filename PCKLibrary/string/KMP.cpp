//prefix[i] = S[0, i + 1) の真の border の最大長。O(N)
template<class T>
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

//text 内で pattern が始まる位置をすべて返す。空 pattern は全境界に一致
template<class T>
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

//S の最小周期。S が周期の整数回の繰り返しでなければ |S|
template<class T>
int minimumPeriod(const T& S) {
    if (S.empty()) return 0;
    vector<int> prefix = prefixFunction(S);
    int period = (int)S.size() - prefix.back();
    if ((int)S.size() % period == 0) return period;
    return (int)S.size();
}

vector<pair<char, int>> runLengthEncode(const string& S) {
    int N = (int)S.size();
    vector<pair<char, int>> ret;
    for (int l = 0; l<N;) {
        int r = l+1;
        for (; r<N && S[l]==S[r]; r++) ;
        ret.emplace_back(S[l], r-l);
        l = r;
    }
    return ret;
}

template<class T>
vector<pair<T, int>> runLengthEncode(const vector<T>& S) {
    int N = (int)S.size();
    vector<pair<T, int>> ret;
    for (int l = 0; l<N;) {
        int r = l+1;
        for (; r<N && S[l]==S[r]; r++) ;
        ret.emplace_back(S[l], r-l);
        l = r;
    }
    return ret;
}

string runLengthDecode(const vector<pair<char, int>>& code) {
    string ret = "";
    for (pair<char, int> c : code) {
        ret+=string(c.first, c.second);
    }
    return ret;
}

template<class T>
vector<T> runLengthDecode(const vector<pair<T, int>>& code) {
    vector<T> ret;
    for (auto[v, len] : code) {
        for (int i = 0; i < len; i++) ret.push_back(v);
    }
    return ret;
}
template<class T>
vector<int> compress(const vector<T>& A) {
    vector<T> B = A;
    sort(B.begin(), B.end());
    B.erase(unique(B.begin(), B.end()), B.end());
    vector<int> res((int)A.size());
    for (int i = 0; i < (int)A.size(); i++) {
        res[i] = lower_bound(B.begin(), B.end(), A[i])-B.begin();
    }
    return res;
}
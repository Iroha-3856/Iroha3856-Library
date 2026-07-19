//vector<T>であらわされるn進数を10進数に変換する
template<class T>
T changeBaseToTen(const vector<T>& A, T n) {
    T ret = 0;
    T mul = 1;
    for (int i = (int)A.size()-1; i >= 0; i--) {
        ret += mul * A[i];
        mul *= n;
    }
    return ret;
}

//10進数でxのものをn進数に変換する
template<class T>
vector<T> changeBaseFromTen(T x, T n) {
    vector<T> ret;
    while(x != 0) {
        ret.push_back(x%n);
        x /= n;
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

//n進法の桁のvectorであるSをm進法に変換する
template<class T>
vector<T> changeBase(const vector<T>& S, T n, T m) {
    T ten = changeBaseToTen(S, n);
    return changeBaseTromTen(ten, m);
} 
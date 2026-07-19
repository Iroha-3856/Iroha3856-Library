//xor基底
//https://atcoder.jp/contests/abc283/editorial/5430 のような基底を作る
template<class T>
vector<T> xorBasis(const vector<T>& A) {
    vector<T> ret;
    for (T a : A) {
        for (T b : ret) {
            if ((a^b) < a) a^=b;
        }
        for (T& b : ret) {
            if ((b^a) < b) b^=a;
        }
        if (a) ret.push_back(a);
    }
    return ret;
}
//1次元累積和
//全て0-indexedで処理
template<class T>
struct CumulativeSum {
    int siz;
    vector<T> S;
    bool done;
    CumulativeSum() : CumulativeSum(0) {}
    CumulativeSum(int N) : CumulativeSum(vector<T>(N, 0)) {}
    //累積和の構築はしない
    CumulativeSum(vector<T> A) {
        done = false;
        siz = (int)A.size();
        S.resize(siz+1);
        S[0] = 0;
        for (int i = 0; i < siz; i++) {
            S[i+1] = A[i];
        }
    }
    //累積
    void build() {
        assert(!done);
        for (int i = 1; i <= siz; i++) {
            S[i] += S[i-1];
        }
        done = true;
    }
    //加算（累積前のみ）
    T add(int idx, T a) {
        assert(!done);
        return S[idx+1] += a;
    }
    //代入（累積前のみ）
    T set(int idx, T a) {
        assert(!done);
        return S[idx+1] = a;
    }
    //取得（累積前、累積後両方とも可能だが、どちらを想定するかをexpected_doneで渡す）
    [[nodiscard]]
    T get(int idx, bool expected_done) {
        assert(expected_done == done);
        return S[idx+1];
    }
    //区間和取得（累積後のみ）
    //半開区間で与える
    [[nodiscard]]
    T sum(int l, int r) {
        assert(done);
        return S[r]-S[l];
    }
};
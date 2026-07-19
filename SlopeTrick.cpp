template<class T, T INF>
struct SlopeTrick {
    priority_queue<T> L;
    priority_queue<T, vector<T>, greater<T>> R;
    T Lsum, Rsum, Minf;
    SlopeTrick() : Lsum(0), Rsum(0) {
        L.push(-INF);
        R.push(INF);
    }

    //定数加算
    void addC(T a) {
        Minf += a;
    }
    //add max(a-x, 0)
    //\_のような関数を追加 折れ線のx座標がa
    void addL(T a) {
        Minf += max(T(0), a - R.top());
        R.push(a);
        Rsum += a; Rsum -= R.top(); Lsum += R.top();
        L.push(R.top()); R.pop();
    }
    //add max(x-a, 0)
    //_/のような関数を追加 折れ線のx座標がa
    void addR(T a) {
        Minf += max(T(0), L.top()-a);
        L.push(a);
        Lsum += a; Lsum -= L.top(); Rsum += L.top();
        R.push(L.top()); L.pop();
    }
    //絶対値の加算
    void addAbs(T a) {
        addL(a);
        addR(a);
    }
    T get_min() {
        return Minf;
    }
};
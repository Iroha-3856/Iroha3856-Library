//depends on SlindingAggregation.cpp

template<class T, T(*op)(T, T)> 
vector<T> SlidingWindow(const vector<T>& A, int L) {
    int N = (int)A.size();
    assert(L <= N);
    vector<T> ret(N);
    SlidingWindowAggregation<T, op> swag;
    for (int i = 0; i < L; i++) swag.push(A[i]);
    for (int i = 0; i <= N-L; i++) {
        ret[i] = swag.prod();
        swag.pop();
        if (i+L < N) swag.push(A[i+L]);
    }
    return ret;
}

template<class T, T(*op)(T, T)>
vector<vector<T>> SlidingRectangle(const vector<vector<T>>& A, int R, int C) {
    int H = (int)A.size();
    int W = (int)A[0].size();
    assert(R <= H); assert(C <= W);
    vector<vector<T>> ret(H);
    for (int i = 0; i < H; i++) ret[i] = SlidingWindow<T, op>(A[i], C);
    for (int j = 0; j < W; j++) {
        vector<T> col(H);
        for (int i = 0; i < H; i++) col[i] = ret[i][j];
        col = SlidingWindow<T, op>(col, R);
        for (int i = 0; i < H; i++) ret[i][j] = col[i];
    }
    return ret;
}